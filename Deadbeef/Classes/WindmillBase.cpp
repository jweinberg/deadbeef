/*
 *  WindmillBase.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/20/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "WindmillBase.h"

namespace Deadbeef
{
	WindmillBase::WindmillBase(b2World& world, Puzzle* parent):
	PuzzlePiece("Windmill:base", world, Vect2(16, 22.5), parent) 
	{
		m_linked = true;
	
		m_soundEffect = "SFX06";
		mp_physicsBody->SetType(b2_staticBody);
		
		// attach shape to fixture
		b2PolygonShape bodyShape;
		bodyShape.SetAsBox(4.9, .4);
		
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
		
		b2CircleShape leftSensorShape;
		leftSensorShape.m_p = b2Vec2(-1.65, -0.5);
		leftSensorShape.m_radius = 0.25;
		
		b2CircleShape rightSensorShape;
		rightSensorShape.m_p = b2Vec2(1.8, -0.5);
		rightSensorShape.m_radius = 0.25;
		
		b2FixtureDef leftSensorDef;
		leftSensorDef.shape = &leftSensorShape;
		leftSensorDef.isSensor = true;
		leftSensorDef.isMagnetic = false;
		leftSensorDef.filter.categoryBits = 0x0002;
		leftSensorDef.filter.maskBits =  0x0002;
		leftSensorDef.userData = this;
		
		b2FixtureDef rightSensorDef;
		rightSensorDef.shape = &rightSensorShape;
		rightSensorDef.isSensor = true;
		rightSensorDef.isMagnetic = false;
		rightSensorDef.filter.categoryBits = 0x0004;
		rightSensorDef.filter.maskBits =  0x0004;
		rightSensorDef.userData = this;
		
		mp_sensorLeft = mp_physicsBody->CreateFixture(&leftSensorDef);
		mp_sensorRight = mp_physicsBody->CreateFixture(&rightSensorDef);
	}
	
	WindmillBase::~WindmillBase()
	{
		
	}
}