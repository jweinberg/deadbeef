/*
 *  Vect2.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

namespace Deadbeef
{
	class Vect2
	{
	public:
		Vect2();
		Vect2(float tx, float ty);
		virtual ~Vect2();
		
		
		const Vect2 operator+(const Vect2& other) const;
		const Vect2 operator-(const Vect2& other) const;
		const Vect2 operator*(const float scaler) const;
		const Vect2 operator/(const float scaler) const;
		
		const Vect2 operator*(const Vect2 &other) const;
		void operator*=(const float scaler);
		void operator=(const Vect2& other);
		const float lengthSquared() const;
		const float length() const;
		const float dot(const Vect2& other) const;
		const float distance(const Vect2& other) const;
		const Vect2 Normalize() const;
		
		float x;
		float y;
	};
}