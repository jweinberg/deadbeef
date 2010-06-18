/*
 *  Matrix.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 4/19/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#import <assert.h>

class Matrix
{
public:
    
	Matrix(int r = 4, int c = 4, float diagVal = 1.0);
	Matrix(const Matrix &m1);
	~Matrix() { delete [] m; }
    
	int Rows(void) { return rows; }
	int Cols(void) { return cols; }
    
	Matrix& operator=(const Matrix &m1) { Copy(m1); return *this; }
    
	float &operator()(unsigned int i, unsigned int j) { assert(i < rows && j < cols); return m[rows * j + i]; }
	float operator()(unsigned int i, unsigned int j) const { assert(i < rows && j < cols); return m[rows * j + i]; }
	float &operator[](int i) { assert((unsigned int)i < rows * cols); return m[i]; }
    
	operator const float *(void) const { return m; }
    
	// Warning:  This method gives unrestricted access to the internal
	// array and is a possiblitity for memory corruption.
	float *buf(void) { return m; }
	float *AsArray(void) { return m; }
	const float *AsArray(void) const { return m; }
    
	void print(void);
    
	void Copy(const Matrix &m1);
	void SwapRows(unsigned int i, unsigned int j);
	void AddRowMultiple(float d, unsigned int i, unsigned int j);
	void AddRowMultiple(float d, unsigned int i, unsigned int j, int startCol);
	void MultiplyRow(float d, unsigned int i);
	void Gauss(void);
	void GaussJordan(void);
	void SolveGaussJordan(Matrix &RHS);
	Matrix Inverse(void);
	float Invert(void);
	Matrix Adjoint3x3(void);
	Matrix Transpose(void);
    
	float Determinant(void);
    
	void Identity(float d = 1.0);
	void Diagonal(float *v);
	void MakeRotationX(float alpha);
	void MakeRotationY(float alpha);
	void MakeRotationZ(float alpha);
	void MakeTranslation(float x, float y, float z);
	void MakeScale(float x, float y, float z);
    void MakeOrthoProjection(float left, float right, float top, float bottom, float near, float far);
    
	friend void multiply(const Matrix &m1, const Matrix &m2, Matrix &result);
	friend void add(const Matrix &m1, const Matrix &m2, Matrix &result);
	friend void subtract(const Matrix &m1, const Matrix &m2, Matrix &result);
	friend Matrix operator*(const Matrix &m1, const Matrix &m2);
    
	Matrix &operator+=(const Matrix &m2);
	Matrix &operator-=(const Matrix &m2);
	Matrix &operator*=(const Matrix m2);
    
private:
    
	unsigned int rows, cols;
	float *m;
};

Matrix operator+(const Matrix &m1, const Matrix &m2);
void add(const Matrix &m1, const Matrix &m2, Matrix &result);
Matrix operator-(const Matrix &m1, const Matrix &m2);
void subtract(const Matrix &m1, const Matrix &m2, Matrix &result);
Matrix operator*(const Matrix &m1, const Matrix &m2);
void multiply(const Matrix &m1, const Matrix &m2, Matrix &result);