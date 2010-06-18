/*
 *  WindmillRightFoot.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/20/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "WindmillRightFoot.h"
#include "WindmillPuzzle.h"
#include "SoundManager.h"

namespace Deadbeef
{
	WindmillRightFoot::WindmillRightFoot(b2World& world, Vect2 position, Puzzle* parent):
	PuzzlePiece("Windmill:rightfoot", world, position, parent)
	{
		m_soundEffect = "SFX06";
		
		// attach shape to fixture
		// attach shape to fixture
		b2PolygonShape bodyShape;
		b2Vec2 verts[5];
		verts[4] = b2Vec2(-1.75, -1.75);
		verts[3] = b2Vec2(-1.75, 1.75);
		verts[2] = b2Vec2( 1.75, 1.75);
		verts[1] = b2Vec2( 1.75, -1.0);
		verts[0] = b2Vec2( 1.2, -1.75);
		
		bodyShape.Set(verts, 5);
		
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
		bottomSensorShape.m_p = b2Vec2(0, 1.75);
		bottomSensorShape.m_radius = 0.2;
		
		b2CircleShape topSensorShape;
		topSensorShape.m_p = b2Vec2(0, -1.75);
		topSensorShape.m_radius = 0.2;
		
		b2FixtureDef bottomSensorDef;
		bottomSensorDef.shape = &bottomSensorShape;
		bottomSensorDef.isSensor = true;
		bottomSensorDef.isMagnetic = false;
		bottomSensorDef.filter.categoryBits = 0x0004;
		bottomSensorDef.filter.maskBits = 0x0004;
		bottomSensorDef.userData = this;
		
		b2FixtureDef topSensorDef;
		topSensorDef.shape = &topSensorShape;
		topSensorDef.isSensor = true;
		topSensorDef.isMagnetic = false;
		topSensorDef.filter.categoryBits = 0x0010;
		topSensorDef.filter.maskBits = 0x0010;
		topSensorDef.userData = this;
		
		b2Fixture* mp_sensorBottom;
		b2Fixture* mp_sensorTop;
		
		mp_sensorBottom = mp_physicsBody->CreateFixture(&bottomSensorDef);
		mp_sensorTop = mp_physicsBody->CreateFixture(&topSensorDef);
	}
	
	WindmillRightFoot::~WindmillRightFoot()
	{
		
	}
	
	void WindmillRightFoot::Update(float elapsedTime)
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
						SoundManager::GetInstance().PlaySound("SFX08", 3.0, false);
						mp_physicsBody->SetType(b2_staticBody);
						mp_physicsBody->SetTransform(b2Vec2(xLock, yLock), 0);
						((WindmillPuzzle*)mp_parentPuzzle)->SetRightFootLocked();
						m_linked = true;
						
						SetImage("Windmill:rightfooton");
						frameRate = 3;
						
					}
				}
				
				contactEdges = contactEdges->next;
			}
		}
		
		PhysicsSprite::Update(elapsedTime);
	}
}