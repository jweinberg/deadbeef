/*
 *  Avatar.h
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 5/8/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#pragma once

#include "GameObject.h"
#include "Sprite.h"
#include "Box2D.h"
#include "PhysicsSprite.h"
#include "Magnet.h"
#include "ParticleEmitter.h"

namespace Deadbeef
{
	class Avatar : public PhysicsSprite
	{
	public:	
		Avatar(b2World& world,Vect2 position);
		virtual ~Avatar();
		
		virtual void Update(float elapsedTime);
		
		// may not be using this if we don't use "desiredPosition"
		bool IsMagnetOn()const {return m_magnetOn;}
		
		void ActivateMagnet();
		void DeactivateMagnet();
		void MoveToTarget(b2Vec2 target);
		void MoveAvatar(b2Vec2 impulse);
		Magnet* GetMagnet()const {return mp_Magnet;}
	
	private:
		b2Vec2 m_desiredPosition;
		Magnet* mp_Magnet;
		SpriteRef mr_testSprite;

		bool m_movingToTarget;
		bool m_magnetOn;
	};
}
