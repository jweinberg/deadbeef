/*
 *  Vect4.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

namespace Deadbeef
{
	class Vect4
	{
	public:
		Vect4();
		Vect4(float tx, float ty, float tz, float tw);
		virtual ~Vect4();
		
		
		const Vect4 operator+(const Vect4& other) const;
		const Vect4 operator-(const Vect4& other) const;
		const Vect4 operator*(const float scaler) const;
		const Vect4 operator/(const float scaler) const;
		
		const float lengthSquared() const;
		const float length() const;
		const float dot(const Vect4& other) const;
		float x;
		float y;
		float z;
		float w;
	};
}