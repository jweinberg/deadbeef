/*
 *  Matrix.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Matrix.h"
#include <math.h>
#include <stdio.h>

namespace Deadbeef
{
	Matrix::Matrix():
	m0(1.0f),m1(0.0f),m2(0.0f),m3(0.0f),
	m4(0.0f),m5(1.0f),m6(0.0f),m7(0.0f),
	m8(0.0f),m9(0.0f),m10(1.0f),m11(0.0f),
	m12(0.0f),m13(0.0f),m14(0.0f),m15(1.0f)
	{
	}

	Matrix::Matrix(const float t0, const float t1, const float t2, const float t3,
					 const float t4, const float t5, const float t6, const float t7,
					 const float t8, const float t9, const float t10, const float t11,  
					 const float t12, const float t13, const float t14, const float t15):
					m0(t0), m1(t1), m2(t2), m3(t3),
					m4(t4), m5(t5), m6(t6), m7(t7),
					m8(t8), m9(t9), m10(t10), m11(t11),
					m12(t12), m13(t13), m14(t14), m15(t15)
	{
	}
	
	Matrix::Matrix(const Matrix& mat):
	m0(mat.m0),m1(mat.m1),m2(mat.m2),m3(mat.m3),
	m4(mat.m4),m5(mat.m5),m6(mat.m6),m7(mat.m7),
	m8(mat.m8),m9(mat.m9),m10(mat.m10),m11(mat.m11),
	m12(mat.m12),m13(mat.m13),m14(mat.m14),m15(mat.m15)
	{
	}
#ifdef _ARM_ARCH_7
	Matrix::Matrix(const float32x4_t t0, const float32x4_t t1, const float32x4_t t2, const float32x4_t t3)
	:v0(t0),v1(t1),v2(t2),v3(t3)
	{
		
	}
#endif
	Matrix::~Matrix()
	{
	}
	
	Matrix Matrix::MakeTranslation(const float tx, const float ty, const float tz)
	{
		return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
					  0.0f, 1.0f, 0.0f, 0.0f,
					  0.0f, 0.0f, 1.0f, 0.0f,
					  tx, ty, tz, 1.0f);
	}
	
	Matrix Matrix::MakeScale(const float sx, const float sy,const float sz)
	{
		return Matrix(sx, 0.0f, 0.0f, 0.0f,
					   0.0f, sy, 0.0f, 0.0f,
					   0.0f, 0.0f, sz, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	Matrix Matrix::MakeRotationX(const float rx)
	{
		const float a = rx * M_PI / 180.0f;
		const float c = cos(a);
		const float s = sin(a);
		
		return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
					   0.0f, c, s, 0.0f,
					   0.0f, -s, c, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);
	}
	 
	Matrix Matrix::MakeRotationY(const float ry)
	{
		const float a = ry * M_PI / 180.0f;
		const float c = cos(a);
		const float s = sin(a);
		
		return Matrix(c, 0.0f, -s, 0.0f,
					   0.0f, 1.0f, 0.0f, 0.0f,
					   s, 0.0f, c, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	Matrix Matrix::MakeRotationZ(const float rz)
	{
		const float a = rz * M_PI / 180.0f;
		const float c = cos(a);
		const float s = sin(a);
		
		return Matrix( c, s, 0.0f, 0.0f,
					   -s, c, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	Matrix Matrix::MakeOrthoProjection(const float left, const float right, const float top, 
										 const float bottom, const float near, const float far)
	{
		return Matrix( 2.0f/(right-left), 0.0f, 0.0f, 0.0f,
						0.0f, 2.0f/(top-bottom), 0.0f, 0.0f,
						0.0f, 0.0f, 2.0f/(far-near), 0.0f,
						-(right + left)/(right - left), -(top + bottom)/(top - bottom), -(far + near)/(far - near), 1.0f);		
	}
	
	
	
	Matrix Matrix::operator*(const Matrix& B) const
	{
#ifdef _ARM_ARCH_7
		const float32x4_t A1 = vmulq_f32(B.v0, vdupq_n_f32(vgetq_lane_f32(v0,0)));
		const float32x4_t B1 = vmulq_f32(B.v1, vdupq_n_f32(vgetq_lane_f32(v0,1)));
		const float32x4_t C1 = vmulq_f32(B.v2, vdupq_n_f32(vgetq_lane_f32(v0,2)));
		const float32x4_t D1 = vmulq_f32(B.v3, vdupq_n_f32(vgetq_lane_f32(v0,3)));
		const float32x4_t _v0 = vaddq_f32( vaddq_f32(A1 , B1), vaddq_f32(C1,  D1) );
		
		const float32x4_t A2 = vmulq_f32(B.v0, vdupq_n_f32(vgetq_lane_f32(v1,0)));
		const float32x4_t B2 = vmulq_f32(B.v1, vdupq_n_f32(vgetq_lane_f32(v1,1)));
		const float32x4_t C2 = vmulq_f32(B.v2, vdupq_n_f32(vgetq_lane_f32(v1,2)));
		const float32x4_t D2 = vmulq_f32(B.v3, vdupq_n_f32(vgetq_lane_f32(v1,3)));
		const float32x4_t _v1 = vaddq_f32( vaddq_f32(A2 , B2),  vaddq_f32(C2 ,  D2) );
		
		const float32x4_t A3 = vmulq_f32(B.v0, vdupq_n_f32(vgetq_lane_f32(v2,0)));
		const float32x4_t B3 = vmulq_f32(B.v1, vdupq_n_f32(vgetq_lane_f32(v2,1)));
		const float32x4_t C3 = vmulq_f32(B.v2, vdupq_n_f32(vgetq_lane_f32(v2,2)));
		const float32x4_t D3 = vmulq_f32(B.v3, vdupq_n_f32(vgetq_lane_f32(v2,3)));
		const float32x4_t _v2 = vaddq_f32( vaddq_f32(A3 , B3),  vaddq_f32(C3 ,  D3) );
		
		const float32x4_t A4 = vmulq_f32(B.v0, vdupq_n_f32(vgetq_lane_f32(v3,0)));
		const float32x4_t B4 = vmulq_f32(B.v1, vdupq_n_f32(vgetq_lane_f32(v3,1)));
		const float32x4_t C4 = vmulq_f32(B.v2, vdupq_n_f32(vgetq_lane_f32(v3,2)));
		const float32x4_t D4 = vmulq_f32(B.v3, vdupq_n_f32(vgetq_lane_f32(v3,3)));
		const float32x4_t _v3 = vaddq_f32( vaddq_f32(A4 , B4),  vaddq_f32(C4,  D4) );

		return Matrix(_v0, _v1, _v2, _v3);
#else
		return Matrix(
					  (m0 * B.m0 + m1 * B.m4 + m2 * B.m8 + m3 * B.m12),
			(m0 * B.m1 + m1 * B.m5 + m2 * B.m9 + m3 * B.m13),
			(m0 * B.m2 + m1 * B.m6 + m2 * B.m10 + m3 * B.m14),
			(m0 * B.m3 + m1 * B.m7 + m2 * B.m11 + m3 * B.m15),
					   
			(m4 * B.m0 + m5 * B.m4 + m6 * B.m8 + m7 * B.m12),
			(m4 * B.m1 + m5 * B.m5 + m6 * B.m9 + m7 * B.m13),
			(m4 * B.m2 + m5 * B.m6 + m6 * B.m10 + m7 * B.m14),
			(m4 * B.m3 + m5 * B.m7 + m6 * B.m11 + m7 * B.m15),
					   
			(m8 * B.m0 + m9 * B.m4 + m10 * B.m8 + m11 * B.m12),
			(m8 * B.m1 + m9 * B.m5 + m10 * B.m9 + m11 * B.m13),
			(m8 * B.m2 + m9 * B.m6 + m10 * B.m10 + m11 * B.m14),
			(m8 * B.m3 + m9 * B.m7 + m10 * B.m11 + m11 * B.m15),
					   
			(m12 * B.m0 + m13 * B.m4 + m14 * B.m8 + m15 * B.m12),
			(m12 * B.m1 + m13 * B.m5 + m14 * B.m9 + m15 * B.m13),
			(m12 * B.m2 + m13 * B.m6 + m14 * B.m10 + m15 * B.m14),
			(m12 * B.m3 + m13 * B.m7 + m14 * B.m11 + m15 * B.m15)
					   );
#endif
	}
    Vect4 Matrix::operator*(const Vect4& B) const
    {
        return Vect4(
                     (B.x * m0 + B.y * m4 + B.z * m8 + B.w * m12),
                     (B.x * m1 + B.y * m5 + B.z * m9 + B.w * m13),
                     (B.x * m2 + B.y * m6 + B.z * m10 + B.w * m14),
                     (B.x * m3 + B.y * m7 + B.z * m11 + B.w * m15));
                      
    }
    
	void Matrix::operator*=(const Matrix& B)
	{
#ifdef _ARM_ARCH_7
		const float32x4_t A1 = vmulq_f32(B.v0, vdupq_n_f32(vgetq_lane_f32(v0,0)));
		const float32x4_t B1 = vmulq_f32(B.v1, vdupq_n_f32(vgetq_lane_f32(v0,1)));
		const float32x4_t C1 = vmulq_f32(B.v2, vdupq_n_f32(vgetq_lane_f32(v0,2)));
		const float32x4_t D1 = vmulq_f32(B.v3, vdupq_n_f32(vgetq_lane_f32(v0,3)));
		const float32x4_t _v0 = vaddq_f32( vaddq_f32(A1 , B1), vaddq_f32(C1,  D1) );
		
		const float32x4_t A2 = vmulq_f32(B.v0, vdupq_n_f32(vgetq_lane_f32(v1,0)));
		const float32x4_t B2 = vmulq_f32(B.v1, vdupq_n_f32(vgetq_lane_f32(v1,1)));
		const float32x4_t C2 = vmulq_f32(B.v2, vdupq_n_f32(vgetq_lane_f32(v1,2)));
		const float32x4_t D2 = vmulq_f32(B.v3, vdupq_n_f32(vgetq_lane_f32(v1,3)));
		const float32x4_t _v1 = vaddq_f32( vaddq_f32(A2 , B2),  vaddq_f32(C2 ,  D2) );
		
		const float32x4_t A3 = vmulq_f32(B.v0, vdupq_n_f32(vgetq_lane_f32(v2,0)));
		const float32x4_t B3 = vmulq_f32(B.v1, vdupq_n_f32(vgetq_lane_f32(v2,1)));
		const float32x4_t C3 = vmulq_f32(B.v2, vdupq_n_f32(vgetq_lane_f32(v2,2)));
		const float32x4_t D3 = vmulq_f32(B.v3, vdupq_n_f32(vgetq_lane_f32(v2,3)));
		const float32x4_t _v2 = vaddq_f32( vaddq_f32(A3 , B3),  vaddq_f32(C3 ,  D3) );
		
		const float32x4_t A4 = vmulq_f32(B.v0, vdupq_n_f32(vgetq_lane_f32(v3,0)));
		const float32x4_t B4 = vmulq_f32(B.v1, vdupq_n_f32(vgetq_lane_f32(v3,1)));
		const float32x4_t C4 = vmulq_f32(B.v2, vdupq_n_f32(vgetq_lane_f32(v3,2)));
		const float32x4_t D4 = vmulq_f32(B.v3, vdupq_n_f32(vgetq_lane_f32(v3,3)));
		const float32x4_t _v3 = vaddq_f32( vaddq_f32(A4 , B4),  vaddq_f32(C4,  D4) );
		
		v0 = _v0;
		v1 = _v1;
		v2 = _v2;
		v3 = _v3;
#else
		
		const float _m0 = (m0 * B.m0 + m1 * B.m4 + m2 * B.m8 + m3 * B.m12);
		const float _m1 = (m0 * B.m1 + m1 * B.m5 + m2 * B.m9 + m3 * B.m13);
		const float _m2 = (m0 * B.m2 + m1 * B.m6 + m2 * B.m10 + m3 * B.m14);
		const float _m3 = (m0 * B.m3 + m1 * B.m7 + m2 * B.m11 + m3 * B.m15);
					  
		const float _m4 = (m4 * B.m0 + m5 * B.m4 + m6 * B.m8 + m7 * B.m12);
		const float _m5 = (m4 * B.m1 + m5 * B.m5 + m6 * B.m9 + m7 * B.m13);
		const float _m6 = (m4 * B.m2 + m5 * B.m6 + m6 * B.m10 + m7 * B.m14);
		const float _m7 = (m4 * B.m3 + m5 * B.m7 + m6 * B.m11 + m7 * B.m15);
					  
		const float _m8 = (m8 * B.m0 + m9 * B.m4 + m10 * B.m8 + m11 * B.m12);
		const float _m9 = (m8 * B.m1 + m9 * B.m5 + m10 * B.m9 + m11 * B.m13);
		const float _m10 = (m8 * B.m2 + m9 * B.m6 + m10 * B.m10 + m11 * B.m14);
		const float _m11 = (m8 * B.m3 + m9 * B.m7 + m10 * B.m11 + m11 * B.m15);
					  
		const float _m12 = (m12 * B.m0 + m13 * B.m4 + m14 * B.m8 + m15 * B.m12);
		const float _m13 = (m12 * B.m1 + m13 * B.m5 + m14 * B.m9 + m15 * B.m13);
		const float _m14 = (m12 * B.m2 + m13 * B.m6 + m14 * B.m10 + m15 * B.m14);
		const float _m15 = (m12 * B.m3 + m13 * B.m7 + m14 * B.m11 + m15 * B.m15);

		m0 = _m0;
		m1 = _m1;
		m2 = _m2;
		m3 = _m3;
		m4 = _m4;
		m5 = _m5;
		m6 = _m6;
		m7 = _m7;
		m8 = _m8;
		m9 = _m9;
		m10 = _m10;
		m11 = _m11;
		m12 = _m12;
		m13 = _m13;
		m14 = _m14;
		m15 = _m15;
#endif
	}
	
	void Matrix::print()
	{
		debug_log("%18.16f,%18.16f,%18.16f,%18.16f\n", m0, m1, m2, m3);
		debug_log("%18.16f,%18.16f,%18.16f,%18.16f\n", m4, m5, m6, m7);
		debug_log("%18.16f,%18.16f,%18.16f,%18.16f\n", m8, m9, m10, m11);
		debug_log("%18.16f,%18.16f,%18.16f,%18.16f\n", m12, m13, m14, m15);	
	}
	
}
