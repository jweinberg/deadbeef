/*
 *  GamePiece.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/16/10.
 *  Copyright 2010  Andy Cook. All rights reserved.
 *
 */

#include "PuzzlePiece.h"

namespace Deadbeef 
{
	
	PuzzlePiece::PuzzlePiece(const std::string& imageName, b2World& world, const Vect2& position, Puzzle* parent): 
	PhysicsSprite(imageName),mp_parentPuzzle(parent),  m_linked(false)
	{
		
		mp_world = &world;
		
		// make body
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(position.x, position.y);
		mp_physicsBody = mp_world->CreateBody(&bodyDef);
		

		rotation = mp_physicsBody->GetAngle() * 180 / M_PI;
	
		debug_log("PuzzlePiece location: (%f, %f)\n", this->position.x,this->position.y);


		//mp_fixture->SetIsMagnetic(true);
	}
	
	PuzzlePiece::~PuzzlePiece()
	{
		
	}

}
