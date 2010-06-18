/*
 *  Matrix.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 4/19/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "MathShortcuts.h"
#include "-Matrix.h"

using namespace std;

Matrix::Matrix(int r, int c, float diagVal)
{
	rows = r;
	cols = c;
	m = new float[r*c];	
	memset(m, 0, r*c*sizeof(float));
	int lim = min(rows, cols);
	for (int i = 0; i < lim; i++)
		m[i*rows + i] = diagVal;
}

Matrix::Matrix(const Matrix &m1) 
{
	rows = m1.rows;
	cols = m1.cols;
	m = new float[rows * cols];
	memcpy(m, m1.m, rows * cols * sizeof(float));
}	

void Matrix::SwapRows(unsigned int i, unsigned int j)
{
	float temp;
	float *d1 = m + i, *d2 = m + j;
	assert(i < rows && j < rows);
    
	if (i == j)
		return;
    
	for (unsigned int k = 0; k < cols; k++, d1 += cols, d2 += cols)
	{
		temp = (*d1);
		(*d1) = (*d2);
		(*d2) = temp;
	}
}

void Matrix::AddRowMultiple(float d, unsigned int i, unsigned int j)
{
	float *d1 = m + i, *d2 = m + j;
	assert(i < rows && j < rows);
    
	for (unsigned int k = 0; k < cols; k++, d1 += cols, d2 += cols)
		(*d2) += d * (*d1);
}

void Matrix::AddRowMultiple(float d, unsigned int i, unsigned  j, int startCol)
{
	float *d1 = m + i + startCol * cols, *d2 = m + j + startCol * cols;
	assert(i < rows && j < rows);
    
	for (unsigned int k = startCol; k < cols; k++, d1 += cols, d2 += cols)
		(*d2) += d * (*d1);
}

void Matrix::MultiplyRow(float d, unsigned int i)
{
	float *d1 = m + i;
	assert(i < rows);
    
	for (unsigned int k = 0; k < cols; k++, d1 += cols)
		(*d1) *= d;
}

void Matrix::Gauss(void)
{
	float pivotVal;
	int pivotIndex;
	float *d1;
    
	for (unsigned int i = 0; i < min(rows, cols); i++)
	{
		d1 = m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		if (fabs(pivotVal) > ALMOST_ZERO)
		{
			SwapRows(i, pivotIndex);
			MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = i + 1; j < rows; j++)
				AddRowMultiple(-m[i*rows + j], i, j, i);
		}
	}
}

void Matrix::GaussJordan(void)
{
	float pivotVal;
	int pivotIndex;
	float *d1;
    
	for (unsigned int i = 0; i < rows; i++)
	{
		d1 = m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		if (fabs(pivotVal) > ALMOST_ZERO)
		{
			SwapRows(i, pivotIndex);
			MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = 0; j < rows; j++)
			{
				if (j == i)
					continue;
				AddRowMultiple(-m[i*rows + j], i, j);
			}
		}
	}
}

float Matrix::Invert(void) // returns the determinant i.e. the product of the pivots
{
	assert(rows == cols);
    
	Matrix inv(rows, cols, 1.0);
	float pivotVal;
	int pivotIndex;
	float *d1;
	float det = 1.0;
    
	for (unsigned int i = 0; i < rows; i++)
	{
		d1 = m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		det *= pivotVal;
		if (fabs(pivotVal) > ALMOST_ZERO)
		{
			SwapRows(i, pivotIndex);
			inv.SwapRows(i, pivotIndex);
			MultiplyRow(1.0f / pivotVal, i);
			inv.MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = 0; j < rows; j++)
			{
				if (j == i)
					continue;
				float val = -m[i*rows + j];
				AddRowMultiple(val, i, j, i);
				inv.AddRowMultiple(val, i, j);
			}
		}
	}
	memcpy(m, inv.m, rows * cols * sizeof(float));
	return det;
}

Matrix Matrix::Inverse(void)
{
	assert(rows == cols);
	Matrix inv(rows, cols, 1.0);
	SolveGaussJordan(inv);
	return inv;
}

Matrix Matrix::Adjoint3x3(void)
{
	assert(rows == 3 && cols == 3);
	
	Matrix temp(3, 3);
	temp.m[0] = m[rows + 1] * m[2*rows + 2] - m[2*rows + 1] * m[rows + 2];
	temp.m[3] = m[2*rows + 1] * m[2] - m[1] * m[2*rows + 2];
	temp.m[6] = m[1] * m[rows + 2] - m[rows + 1] * m[2];
	temp.m[1] = m[rows + 2] * m[2*rows] - m[2*rows + 2] * m[rows];
	temp.m[4] = m[2*rows + 2] * m[0] - m[2] * m[2*rows];
	temp.m[7] = m[2] * m[rows] - m[rows + 2] * m[0];
	temp.m[2] = m[rows] * m[2*rows + 1] - m[2*rows] * m[rows + 1];
	temp.m[5] = m[2*rows] * m[1] - m[0] * m[2*rows + 1];
	temp.m[8] = m[0] * m[rows + 1] - m[rows] * m[1];
    
	return temp;
}

void Matrix::SolveGaussJordan(Matrix &RHS)
{
	assert(RHS.rows = rows);
    
	Matrix temp(*this);
	float pivotVal;
	int pivotIndex;
	float *d1;
    
	for (unsigned int i = 0; i < rows; i++)
	{
		d1 = temp.m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		if (fabs(pivotVal) > ALMOST_ZERO)
		{
			temp.SwapRows(i, pivotIndex);
			RHS.SwapRows(i, pivotIndex);
			temp.MultiplyRow(1.0f / pivotVal, i);
			RHS.MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = 0; j < rows; j++)
			{
				if (j == i)
					continue;
				float val = -temp.m[i*rows + j];
				temp.AddRowMultiple(val, i, j, i);
				RHS.AddRowMultiple(val, i, j);
			}
		}
	}
}

float Matrix::Determinant(void)
{
	Matrix temp(*this);
	float pivotVal;
	int pivotIndex;
	float *d1;
	float det = 1.0;
    
	for (unsigned int i = 0; i < rows; i++)
	{
		d1 = temp.m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		det *= pivotVal;
		if (fabs(pivotVal) > ALMOST_ZERO)
		{
			temp.SwapRows(i, pivotIndex);
			temp.MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = 0; j < rows; j++)
			{
				if (j == i)
					continue;
				float val = -temp.m[i*rows + j];
				temp.AddRowMultiple(val, i, j, i);
			}
		}
	}
	return det;
}

Matrix Matrix::Transpose(void)
{
	Matrix t(rows, cols);
	for (unsigned int i = 0; i < rows; i++)
		for (unsigned int j = 0; j < cols; j++)
			t(i, j) = (*this)(j, i);
	return t;
}

void Matrix::Copy(const Matrix &m1)
{
	if (m1.rows * m1.cols != rows * cols)
	{
		delete [] m;
		m = new float[m1.rows * m1.cols];
	}
	rows = m1.rows;
	cols = m1.cols;
	memcpy(m, m1.m, rows * cols * sizeof(float));
}

/*void Matrix::print(void) 
{
	for (unsigned int i = 0; i < rows; i++)
	{
		for (unsigned int j = 0; j < cols; j++)
		{
			cout << int(1000000 * m[rows * j + i] + .5) / 1000000.0 << "\t";
		}
		cout << endl;
	}
}*/

Matrix &Matrix::operator+=(const Matrix &m2)
{
	const float *d1 = m, *d2 = m2.m;
	float *dr = m;
	int lim = rows * cols;
    
	assert(rows == m2.rows && cols == m2.cols);
    
	for (int i = 0; i < lim; i++, dr++, d1++, d2++)
		(*dr) = (*d1) + (*d2);
	return *this;
}

Matrix operator+(const Matrix &m1, const Matrix &m2)
{
	Matrix result(m1);
	result += m2;
	return result;
}

void add(const Matrix &m1, const Matrix &m2, Matrix &result)
{
	result = m1;
	result += m2;
}

Matrix &Matrix::operator-=(const Matrix &m2)
{
	const float *d1 = m, *d2 = m2.m;
	float *dr = m;
	int lim = rows * cols;
    
	assert(rows == m2.rows && cols == m2.cols);
    
	for (int i = 0; i < lim; i++, dr++, d1++, d2++)
		(*dr) = (*d1) - (*d2);
	return *this;
}

Matrix operator-(const Matrix &m1, const Matrix &m2)
{
	Matrix result(m1);
	result -= m2;
	return result;
}

void subtract(const Matrix &m1, const Matrix &m2, Matrix &result)
{
	result = m1;
	result -= m2;
}

Matrix &Matrix::operator*=(const Matrix m2)
{
	Matrix result(*this);
	multiply(*this, m2, result);
	*this = result;
	return *this;
}

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
	Matrix result(m1.rows, m2.cols);
	multiply(m1, m2, result);
	return result;
}

void multiply(const Matrix &m1, const Matrix &m2, Matrix &result)
{
	const float *d1, *d2;
	float *dr;
    
	assert(m1.cols == m2.rows);
	assert(result.rows == m1.rows);
	assert(result.cols == m2.cols);
    
	dr = result.m;
	for (unsigned int j = 0; j < m2.cols; j++)
	{
		for (unsigned int i = 0; i < m1.rows; i++)
		{
			d1 = m1.m + i;
			d2 = m2.m + j * m2.rows;
			*dr = 0.0;
			for (unsigned int k = 0; k < m1.cols; k++)
			{
				*dr += (*d1) * (*d2);
				d1 += m1.rows;
				d2++;
			}
			dr++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// Matrix Creation
////////////////////////////////////////////////////////////////////////////

void Matrix::Identity(float d) // d defaults to 1.0
{
	int lim = min(rows, cols);
    
	memset(m, 0, rows * cols * sizeof(float));
	for (int i = 0; i < lim; i++)
		m[i*rows + i] = d;
}

void Matrix::Diagonal(float *v)
{
	int lim = min(rows, cols);
    
	memset(m, 0, rows * cols * sizeof(float));
	for (int i = 0; i < lim; i++)
		m[i*rows + i] = v[i];
}

////////////////////////////////////////////////////////////////////////////
// 3D Routines
////////////////////////////////////////////////////////////////////////////

void Matrix::MakeRotationX(float alpha)
{
    
    float a = alpha * M_PI / 180.0f;
    float c = cos(a);
    float s = sin(a);
    
	if (rows != 4 || cols != 4)
    {
        delete [] m;
        (*this) = Matrix(4, 4, 0.0);
    }
    else 
    {
         memset(m, 0, 16*sizeof(float));
    }

   
    m[0] = 1.0;
    m[5] = m[10] = c;
    m[6] = s;
    m[9] = -s;
    m[15] = 1.0;
}

void Matrix::MakeRotationY(float alpha)
{
    float a = alpha * M_PI / 180.0f;
    float c = cos(a);
    float s = sin(a);
    
	if (rows != 4 || cols != 4)
    {
        delete [] m;
        (*this) = Matrix(4, 4, 0.0);
    }
    else 
    {
        memset(m, 0, 16*sizeof(float));
    }
    
    m[5] = 1.0;
    m[0] = m[10] = c;
    m[2] = -s;
    m[8] = s;
    m[15] = 1.0;
}

void Matrix::MakeRotationZ(float alpha)
{
    float a = alpha * M_PI / 180.0f;
    float c = cos(a);
    float s = sin(a);
    
	if (rows != 4 || cols != 4)
    {
        delete [] m;
        (*this) = Matrix(4, 4, 0.0);
    }
    else 
    {
        memset(m, 0, 16*sizeof(float));
    }
    
    m[10] = 1.0;
    m[0] = m[5] = c;
    m[1] = s;
    m[4] = -s;
    m[15] = 1.0;
}

#define sgn(x) ((x) < 0 ? -1 : 1)



void Matrix::MakeTranslation(float x, float y, float z)
{
	if (rows != 4 || cols != 4)
    {
        delete [] m;
		(*this) = Matrix(4, 4, 1.0);
    }
    else 
    {
        Identity();
    }

    m[12] = x;
	m[13] = y;
	m[14] = z;
}

void Matrix::MakeScale(float x, float y, float z)
{
	if (rows != 4 || cols != 4)
    {
        delete [] m;
        (*this) = Matrix(4, 4, 1.0);
    }
    else 
    {
        Identity();
    }

	m[0] = x;
	m[5] = y;
	m[10] = z;
}

void Matrix::MakeOrthoProjection(float left, float right, float top, float bottom, float near, float far)
{
    if (rows != 4 || cols != 4)
    {
        delete [] m;
        (*this) = Matrix(4, 4, 1.0);
    }
    else 
    {
        Identity(1.0);
    }

    (*this)(0,0) = 2/(right - left);
    (*this)(1,1)= 2/(top - bottom);
    (*this)(2,2) = 2/(far - near);
    (*this)(3,3) = 1;
    
    (*this)(0,3) = -(right + left)/(right - left); 
    (*this)(1,3)= -(top + bottom)/(top - bottom);
    (*this)(2,3)= -(far + near)/(far - near);

}

void Matrix::print()
{
	debug_log("%18.16f,%18.16f,%18.16f,%18.16f\n", m[0], m[1], m[2], m[3]);
	debug_log("%18.16f,%18.16f,%18.16f,%18.16f\n", m[4], m[5], m[6], m[7]);
	debug_log("%18.16f,%18.16f,%18.16f,%18.16f\n", m[8], m[9], m[10], m[11]);
	debug_log("%18.16f,%18.16f,%18.16f,%18.16f\n", m[12], m[13], m[14], m[15]);
}




