/*
 *  ContactListener.h
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 5/21/10.
 *  Copyright 2010 DePaul University. All rights reserved.
 *
 */
#pragma once
#include "b2WorldCallbacks.h"

namespace Deadbeef
{
	class ContactListener : public b2ContactListener
	{
	public:
		ContactListener();
		~ContactListener();
		
		//void BeginContact(b2Contact* contact);
		//void EndContact(b2Contact* contact);
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	};
}