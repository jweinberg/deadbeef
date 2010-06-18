/*
 *  Magnet.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/12/10.
 *  Copyright 2010  . All rights reserved.
 *
 */
#pragma once

#include "PhysicsSprite.h"

namespace Deadbeef
{
	class Magnet : public PhysicsSprite
	{		
	public:	
		Magnet(b2World& world, Vect2 position, float fieldStength, float fieldRadius);
		~Magnet();
		void Update(float elapsedTime);
		void Render(float elapsedTime, const Matrix& orientation) const;
		void Enable();
		void Disable();
		
	protected:
		b2Body * mp_nearestBody;
		float m_fieldStrength;
		float m_fieldRadius;
		bool m_enabled;
		bool m_clipClosest;
		Sprite * mp_connector;
		b2Fixture* mp_fieldFixture;
	};
}