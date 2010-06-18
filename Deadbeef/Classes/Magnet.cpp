/*
 *  Magnet.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/12/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "Magnet.h"

namespace Deadbeef
{
	Magnet::Magnet(b2World& world, Vect2 position, float strength, float radius) : PhysicsSprite("Balloon:magnetoff"), 
	m_fieldStrength(strength), m_fieldRadius(radius), m_enabled(false)
	{	
		depth = -1;
		mp_world = &world;
		
		// setting up bodyDef only
		// must call createBody to set the parent classes members
		mp_nearestBody = 0;
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(position.x, position.y);
		mp_physicsBody = mp_world->CreateBody(&bodyDef);
		
		m_clipClosest = false;
	
		// setting up bodyDef only
		// must call createBody to set the parent classes members
		
	
		//-------------------------------------//
		//Setup Solid Fixture 
		//-------------------------------------//
		
		//Define the shape of the solid portion
		b2PolygonShape solidShape;
		solidShape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
	
	
		// fixure definition for describing all attributes assigned to this object
		b2FixtureDef solidFixture;
		solidFixture.shape = &solidShape;
		solidFixture.restitution = .01; // 1.0 very bouncy =]
		solidFixture.density = 0.1f;// needs to be light for balloon to hold
		solidFixture.friction = 2.0;
		
		
		//-------------------------------------//
		//Setup Magnetic Field Sensor Fixture 
		//-------------------------------------//
		//Define the shape of the magentic field
		b2CircleShape fieldShape;
		fieldShape.m_radius = m_fieldRadius;
		
		b2FixtureDef fieldSendorFixture;
		fieldSendorFixture.shape = &fieldShape;
		fieldSendorFixture.isSensor = true;
		fieldSendorFixture.filter.categoryBits = 0x0001;
		fieldSendorFixture.filter.maskBits = 0x0001; 

		mp_fixture =  mp_physicsBody->CreateFixture(&solidFixture);
		mp_fieldFixture = mp_physicsBody->CreateFixture(&fieldSendorFixture);
		
		mp_connector = new Sprite("Balloon:connector");
		mp_connector->position = Vect2(0, -.8f);
		mp_connector->scale = Vect2(1.0f, .60f);
	}
	
	Magnet::~Magnet()
	{
		
	}
	
	void Magnet::Enable()
	{
		SetImage("Balloon:magneton");
		m_enabled = true;
	}
	
	void Magnet::Disable()
	{
		SetImage("Balloon:magnetoff");
		m_enabled = false; 
		m_clipClosest = false; 
		mp_nearestBody = 0;
	}
	
	void Magnet::Render(float elapsedTime, const Matrix& orientation) const
	{
		Matrix parent = this->WorldMatrix();
		mp_connector->Render(elapsedTime, parent * orientation);
		PhysicsSprite::Render(elapsedTime, orientation);
	}
	
	void Magnet::Update(float elapsedTime)
	{
	
		// if there's a registered nearest body then only pull that one
		if(m_enabled && mp_nearestBody != 0)
		{
			b2Vec2 toMagnet = mp_physicsBody->GetPosition() - mp_nearestBody->GetPosition();
			float denom = sqrtf(toMagnet.x * toMagnet.x + toMagnet.y * toMagnet.y);
			if (toMagnet.Length() > m_fieldRadius)
			{
				mp_nearestBody = 0;
				return;
			}
			if (denom > b2_epsilon) 
			{
				float invsLength = 1/denom;
				float magStrength = invsLength * m_fieldRadius;
				toMagnet.x *= invsLength * magStrength;
				toMagnet.y *= invsLength * magStrength;
				
				mp_nearestBody->ApplyLinearImpulse(toMagnet , mp_nearestBody->GetPosition());
				mp_physicsBody->ApplyLinearImpulse(-1 * toMagnet , mp_physicsBody->GetPosition());
			}
			

		}
		//Iterate through contactEdge list
		else if (m_enabled && mp_physicsBody) 
		{
			//Get a linked list of contact edges
			b2ContactEdge* contactEdges =  0;
			contactEdges = mp_physicsBody->GetContactList();
			b2Vec2 closest(32, 24);
			//b2Body * nearestBody;
			if (contactEdges && contactEdges->contact->GetFixtureA()->IsMagnetic())
			{
				mp_nearestBody = contactEdges->contact->GetFixtureA()->GetBody();
				closest = mp_physicsBody->GetPosition() - 
						  mp_nearestBody->GetPosition();
			}
			while(contactEdges) 
			{
				//FixtureB is the magnets
				if(contactEdges->contact->GetFixtureB()->IsSensor())
				{
					if(contactEdges->contact->GetFixtureA()->IsMagnetic() && m_clipClosest == false)
					{
						b2Body* objectBody = contactEdges->contact->GetFixtureA()->GetBody();
						b2Vec2 toMagnet = mp_physicsBody->GetPosition() - objectBody->GetPosition();
						if (toMagnet.Length() < closest.Length())
						{
							closest = toMagnet;
							mp_nearestBody = objectBody;
						}
						debug_log("box body size (%f, %f)\n", this->size.x, this->size.y);
						//Create magnetic pull vector
						float invsLength = 1/sqrtf(toMagnet.x * toMagnet.x + toMagnet.y * toMagnet.y);
						float magStrength = invsLength * m_fieldRadius;
						toMagnet.x *= invsLength * magStrength;
						toMagnet.y *= invsLength * magStrength;
						
						//Apply pull to only one object
						objectBody->ApplyLinearImpulse(toMagnet , objectBody->GetPosition());
						mp_physicsBody->ApplyLinearImpulse(-1 * toMagnet , mp_physicsBody->GetPosition());
						
						// once the first objects breaks this range, it shuts off pull to other objects
						// range is temporary
						if (closest.Length() < 2.50f) 
						{
							m_clipClosest = true;
						}
					}
				}
				contactEdges = contactEdges->next;
			}
		}
		PhysicsSprite::Update(elapsedTime);
	}	
}
