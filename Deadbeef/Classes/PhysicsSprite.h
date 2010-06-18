/*
 *  PhysicsSprite.h
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 5/1/10.
 *  Copyright 2010. All rights reserved.
 *
 */
// 0 - 32 width
// 0 - 24 Height
#pragma once
#include "Sprite.h"
#include "Box2D.h"
#include "string"

namespace Deadbeef
{
	enum body_type
	{
		CIRCLE = 0,
		BOX = 1,
		POLYGON = 2
	};
	class PhysicsSprite : public Sprite
	{
	public:			
		PhysicsSprite(const std::string& spriteName);
		
		PhysicsSprite(const std::string& spriteName, b2World& world, const Vect2& pos );
		
		PhysicsSprite(const std::string& spriteName, b2World& world, const Vect2& pos , b2FixtureDef& fixDef, body_type kBodyType );
		
		virtual ~PhysicsSprite();
		
		void SetBodyAndFixture(b2World& world, b2FixtureDef& fixDef, b2BodyDef& bodyDef);
		
		virtual void Update(float elapsedTime);
		
		virtual void Render(float elapsedTime, const Matrix& projection) const;
		b2Body * GetBody(){return mp_physicsBody;}
		
		bool IsMagnetic()const {return isMagnetic;}
		bool IsAntiGravApplied()const {return antiGravApplied;}
		
		void SetIsMagnetic(const bool value){isMagnetic = value;}
		void SetAntiGrav();
		void PlaySound(float vol, bool loop);
		
		b2World * mp_world;
		b2Body * mp_physicsBody;
		b2Fixture * mp_fixture;
		
		std::string m_soundEffect;
		
	protected:
		bool isMagnetic;		
		bool antiGravApplied;
		
	
	};
}