/*
 *  Vect2.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Vect2.h"
#include <math.h>

namespace Deadbeef
{
	Vect2::Vect2() : x(0), y(0)
	{
	}
	
	Vect2::Vect2(float tx, float ty) : x(tx), y(ty)
	{
	}

	Vect2::~Vect2()
	{
	}

	const Vect2 Vect2::operator+(const Vect2& other) const
	{
		return Vect2(x + other.x, y + other.y);
	}

	const Vect2 Vect2::operator-(const Vect2& other) const
	{
		return Vect2(x - other.x, y - other.y);
	}

	const Vect2 Vect2::operator*(const float scaler) const
	{
		return Vect2(x * scaler, y * scaler);
	}
	
	const Vect2 Vect2::operator*(const Vect2 &other) const
	{
		return Vect2(x * other.x, y * other.y);
	}

	void Vect2::operator=(const Vect2& other)
	{
		this->x = other.x;
		this->y = other.y;
	}
	
	void Vect2::operator*=(const float scaler)
	{
		x *= scaler;
		y *= scaler;
	}
	
	const Vect2 Vect2::operator/(const float scaler) const
	{
		return Vect2(x / scaler, y / scaler);
	}

	const float Vect2::dot(const Vect2& other) const
	{
		return x * other.x + y * other.y;
	}

	const float Vect2::lengthSquared() const
	{
		return x * x + y * y;
	}

	const float Vect2::length() const
	{
		return sqrt(this->lengthSquared());
	}
	
	const float Vect2::distance(const Vect2& other) const
	{
		const float a = other.x - x;
		const float b = other.y - y;
		return sqrt((a*a) + (b*b));
	}
	// returns a Normalized vect2 from the current vect2
	// doesn't change anything
	const Vect2 Vect2::Normalize()const
	{
		float dist = this->length();
		//if (dist > 0) 
		//{
			return Vect2(x/dist, y/dist);
		//}
		
	}
}