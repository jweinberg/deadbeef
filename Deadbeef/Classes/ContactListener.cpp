/*
 *  ContactListener.cpp
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 5/21/10.
 *  Copyright 2010 DePaul University. All rights reserved.
 *
 */

#include "ContactListener.h"
#include "stdio.h"
#include "Box2D.h"
#include "SoundManager.h"
#include "PhysicsSprite.h"

namespace Deadbeef
{
	ContactListener::ContactListener()
	{
	}
	ContactListener::~ContactListener()
	{
	}
#define MIN(A,B) ((A) < (B) ? (A) : (B))
	void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);
		b2PointState state1[2], state2[2];
		b2GetPointStates(state1, state2, oldManifold, contact->GetManifold());
		if (state2[0] == b2_addState)
		{
			const b2Body* bodyA = contact->GetFixtureA()->GetBody();
			const b2Body* bodyB = contact->GetFixtureB()->GetBody();
			b2Vec2 point = worldManifold.points[0];
			b2Vec2 vA = bodyA->GetLinearVelocityFromWorldPoint(point);
			b2Vec2 vB = bodyB->GetLinearVelocityFromWorldPoint(point);
			b2Vec2 dist = vB - vA;
			worldManifold.normal;
			float32 approachVelocity = b2Dot(dist, worldManifold.normal);
			debug_log("approachVelocity = %f\n", approachVelocity);
			if (abs(approachVelocity) > 2.0f)
			{
				float volume = MIN(abs(approachVelocity)/5.0f, 5.0);
				if (contact->GetFixtureA()->GetPhysicsSprite())
					contact->GetFixtureA()->GetPhysicsSprite()->PlaySound(volume, false);
				//SoundManager::GetInstance().PlaySound("SFX03", MIN(abs(approachVelocity)/5.0f, 5.0f), false);
			}
		}
	}
	//void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
//	{
//	}
//	void ContactListener::BeginContact(b2Contact* contact)
//	{
//	}
//	void ContactListener::EndContact(b2Contact* contact)
//	{
//	}
}