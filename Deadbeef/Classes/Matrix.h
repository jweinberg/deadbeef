/*
 *  Matrix.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#import "Vect4.h"

#if !TARGET_IPHONE_SIMULATOR
#import <arm/arch.h>
#endif

#ifdef _ARM_ARCH_7
#import <arm_neon.h>
#endif

namespace Deadbeef
{
	class Matrix
	{
	public:
		Matrix();
		virtual ~Matrix();
		Matrix(const Matrix& mat);
		Matrix(const float t0, const float t1, const float t2, const float t3,
				const float t4, const float t5, const float t6, const float t7,
				const float t8, const float t9, const float t10, const float t11,  
				const float t12, const float t13, const float t14, const float t15);
#ifdef _ARM_ARCH_7		
		Matrix(const float32x4_t t0, const float32x4_t t1, const float32x4_t t2, const float32x4_t t3);
#endif
		static Matrix MakeTranslation(const float tx, const float ty, const float tz);
		static Matrix MakeScale(const float sx, const float sy,const float sz);
		static Matrix MakeRotationX(const float rx);
		static Matrix MakeRotationY(const float ry);
		static Matrix MakeRotationZ(const float rz);
		static Matrix MakeOrthoProjection(const float left, const float right, const float top, 
										   const float bottom, const float near, const float far);
		float &operator[](int i) { return m[i]; }
		Matrix operator*(const Matrix& B) const;
        Vect4 operator*(const Vect4& B) const;
		void operator*=(const Matrix& B);
		void print();
		union
		{
			float m[16];
#ifdef _ARM_ARCH_7
			struct
			{
				float32x4_t v0;
				float32x4_t v1;
				float32x4_t v2;
				float32x4_t v3;
			};
#endif
			struct
			{
				float m0;
				float m1;
				float m2;
				float m3;
				float m4;
				float m5;
				float m6;
				float m7;
				float m8;
				float m9;
				float m10;
				float m11;
				float m12;
				float m13;
				float m14;
				float m15;
			};

		};
	};
}
