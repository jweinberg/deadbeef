/*
 *  WindmillHead.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/20/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "WindmillHead.h"
#include "WindmillPuzzle.h"
#include "SoundManager.h"
namespace Deadbeef
{
	WindmillHead::WindmillHead(b2World& world, Vect2 position, Puzzle* parent):
	PuzzlePiece("Windmill:head", world, position, parent) 
	{
        this->depth = -20;
		m_soundEffect = "SFX06";
		
		// attach shape to fixture
		b2PolygonShape bodyShape;
		b2Vec2 verts[8];
		verts[0] = b2Vec2( 1.6, -1.5);
		verts[1] = b2Vec2( 3.4, -0.4);
		verts[2] = b2Vec2( 3.4, 1.0);
		verts[3] = b2Vec2( 3.0, 1.4);
		verts[4] = b2Vec2(-3.0, 1.4);
		verts[5] = b2Vec2(-3.4, 1.0);
		verts[6] = b2Vec2(-3.4, -0.4);
		verts[7] = b2Vec2(-1.6, -1.5);
		
		
		bodyShape.Set(verts, 8);
		
		// fixure definition for describing all attributes assigned to this object
		b2FixtureDef fd;
		fd.restitution = .01; // 1.0 very bouncy =]
		fd.density = 10.0f;
		fd.shape = &bodyShape;
		fd.friction = 4.5;
		fd.isMagnetic = true;
		
		//Create Main fixture
		mp_fixture = mp_physicsBody->CreateFixture(&fd);
		
		b2MassData mass;
		mass.center = b2Vec2(0,0);
		mass.I = 5.0f;
		mass.mass = 3.0;
		
		mp_physicsBody->SetMassData(&mass);
		
		b2CircleShape bottomSensorShape;
		bottomSensorShape.m_p = b2Vec2(0, 1.5);
		bottomSensorShape.m_radius = 0.2;
		
		b2FixtureDef bottomSensorDef;
		bottomSensorDef.shape = &bottomSensorShape;
		bottomSensorDef.isSensor = true;
		bottomSensorDef.isMagnetic = false;
		bottomSensorDef.filter.categoryBits = 0x0040;
		bottomSensorDef.filter.maskBits = 0x0040;
		bottomSensorDef.userData = this;
		
		b2Fixture* mp_sensorBottom;
		
		bladeSprite = new Deadbeef::Sprite("Windmill:blades");
		bladeSprite->depth = -20;
		bladeSprite->scale = Vect2(0, 0);
		
		mp_sensorBottom = mp_physicsBody->CreateFixture(&bottomSensorDef);
	}
	
	
	WindmillHead::~WindmillHead()
	{
		
	}
	
	void WindmillHead::Update(float elapsedTime)
	{
		
		//Iterate through contactEdge list
		if (mp_physicsBody) 
		{
			//Get a linked list of contact edges
			b2ContactEdge* contactEdges =  0;
			contactEdges = mp_physicsBody->GetContactList();
			
			while(contactEdges) 
			{
				//FixtureB is the magnets
				if(contactEdges->contact->GetFixtureB()->IsSensor() &&
				   contactEdges->contact->GetFixtureA()->IsSensor())
				{
					if (((PuzzlePiece*)contactEdges->contact->GetFixtureA()->GetUserData())->IsLinked() )
					{
						mp_physicsBody->SetType(b2_staticBody);
						mp_physicsBody->SetTransform(b2Vec2(xLock, yLock), 0);
						m_linked = true;
						SoundManager::GetInstance().PlaySound("SFX08", 3.0, false);
						
						mp_parentPuzzle->SetComplete();
						
						SetImage("Windmill:headon");
						frameRate = 3;
					}
				}
				
				contactEdges = contactEdges->next;
			}
		}
		
		bladeSprite->position = position;
		
	if (m_linked) 
	{
		if (bladeSprite->scale.x < 1) 
		{
			bladeSprite->scale = bladeSprite->scale + Vect2(elapsedTime/3, elapsedTime/3);
		}
		else if (bladeSprite->scale.x >= 1) 
		{
			bladeSprite->scale = Vect2(1,1);
			bladeSprite->rotation += elapsedTime * 10;
		}
	}

		
		PhysicsSprite::Update(elapsedTime);
	}
	
	void WindmillHead::Render(float elapsedTime, const Matrix& projection) const
	{
		if (m_linked) 
		{
			bladeSprite->Render(elapsedTime, projection);
		}
		
		Sprite::Render(elapsedTime, projection);
	}
}