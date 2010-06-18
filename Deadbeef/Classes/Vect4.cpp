/*
 *  Vect4.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Vect4.h"

#include <math.h>

namespace Deadbeef
{
	Vect4::Vect4() : x(0), y(0), z(0), w(0)
	{
	}
	
	Vect4::Vect4(float tx, float ty, float tz, float tw) : x(tx), y(ty), z(tz), w(tw)
	{
	}
	
	Vect4::~Vect4()
	{
	}
	
	const Vect4 Vect4::operator+(const Vect4& other) const
	{
		return Vect4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	
	const Vect4 Vect4::operator-(const Vect4& other) const
	{
		return Vect4(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	
	const Vect4 Vect4::operator*(const float scaler) const
	{
		return Vect4(x * scaler, y * scaler, z * scaler, w * scaler);
	}
	
	const Vect4 Vect4::operator/(const float scaler) const
	{
		return Vect4(x / scaler, y / scaler, z / scaler, w / scaler);
	}
	
	const float Vect4::dot(const Vect4& other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
	
	const float Vect4::lengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}
	
	const float Vect4::length() const
	{
		return sqrt(this->lengthSquared());
	}
}