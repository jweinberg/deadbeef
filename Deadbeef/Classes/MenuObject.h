/*
 *  MenuObject.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "PhysicsSprite.h"

namespace Deadbeef
{
	typedef enum
	{
		kLetterG,
		kLetterR,
		kLetterO,
		kLetterU,
		kLetterN,
		kLetterD,
		kLetterP,
		
		kPlay,
	}kLetter;
	
	class MenuObject : public PhysicsSprite
	{
	public:
		static MenuObject* CreateMenuLetter(kLetter letter, b2World& world, const Vect2 &pos);
		
		MenuObject(const std::string & name);
		~MenuObject();
		
	};
}