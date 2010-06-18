/*
 *  WindmillKnees.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/20/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "WindmillKnees.h"
#include "WindmillPuzzle.h"
#include "SoundManager.h"

namespace Deadbeef
{
	WindmillKnees::WindmillKnees(b2World& world, Vect2 position, Puzzle* parent):
	PuzzlePiece("Windmill:knees", world, position, parent)
	{
	
		m_soundEffect = "SFX06";
		
		// attach shape to fixture
		b2PolygonShape bodyShape;
		b2Vec2 verts[8];
		verts[7] = b2Vec2(-3.2, -1.0);
		verts[6] = b2Vec2(-3.5, -0.7);
		verts[5] = b2Vec2(-3.5, 0.7);
		verts[4] = b2Vec2(-3.2, 1.0);
		verts[3] = b2Vec2( 3.2, 1.0);
		verts[2] = b2Vec2( 3.5, 0.7);
		verts[1] = b2Vec2( 3.5, -0.7);
		verts[0] = b2Vec2( 3.2, -1.0);
		
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
		
		b2CircleShape bottomLeftSensorShape;
		bottomLeftSensorShape.m_p = b2Vec2(-1.6, 1);
		bottomLeftSensorShape.m_radius = 0.2;
		
		b2CircleShape bottomRightSensorShape;
		bottomRightSensorShape.m_p = b2Vec2(1.6, 1);
		bottomRightSensorShape.m_radius = 0.2;
		
		b2CircleShape topCenterSensorShape;
		topCenterSensorShape.m_p = b2Vec2(0, -1);
		topCenterSensorShape.m_radius = 0.2;
		
		b2FixtureDef leftSensorDef;
		leftSensorDef.shape = &bottomLeftSensorShape;
		leftSensorDef.isSensor = true;
		leftSensorDef.isMagnetic = false;
		leftSensorDef.filter.categoryBits = 0x0008;
		leftSensorDef.filter.maskBits =  0x0008;
		leftSensorDef.userData = this;
		
		b2FixtureDef rightSensorDef;
		rightSensorDef.shape = &bottomRightSensorShape;
		rightSensorDef.isSensor = true;
		rightSensorDef.isMagnetic = false;
		rightSensorDef.filter.categoryBits = 0x0010;
		rightSensorDef.filter.maskBits =  0x0010;
		rightSensorDef.userData = this;
		
		b2FixtureDef centerSensorDef;
		centerSensorDef.shape = &topCenterSensorShape;
		centerSensorDef.isSensor = true;
		centerSensorDef.isMagnetic = false;
		centerSensorDef.filter.categoryBits = 0x0020;
		centerSensorDef.filter.maskBits =  0x0020;
		centerSensorDef.userData = this;
		
		mp_sensorBottomLeft = mp_physicsBody->CreateFixture(&leftSensorDef);
		mp_sensorBottomRight = mp_physicsBody->CreateFixture(&rightSensorDef);
		mp_sensorTopCenter = mp_physicsBody->CreateFixture(&centerSensorDef);
	}
	
	WindmillKnees::~WindmillKnees()
	{
		
	}
	
	void WindmillKnees::Update(float elapsedTime)
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
						if( ((WindmillPuzzle*)mp_parentPuzzle)->IsLeftFootLocked() && 
						   ((WindmillPuzzle*)mp_parentPuzzle)->IsRightFootLocked() )
						{
							SoundManager::GetInstance().PlaySound("SFX08", 3.0, false);
							mp_physicsBody->SetType(b2_staticBody);
							mp_physicsBody->SetTransform(b2Vec2(xLock, yLock), 0);
							m_linked = true;
							
							SetImage("Windmill:kneeson");
							frameRate = 3;
						}
					}
				}
				
				contactEdges = contactEdges->next;
			}
		}
		
		PhysicsSprite::Update(elapsedTime);
	}
}