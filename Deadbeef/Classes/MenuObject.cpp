/*
 *  MenuObject.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MenuObject.h"

namespace Deadbeef
{

	MenuObject* MenuObject::CreateMenuLetter(kLetter letter, b2World& world, const Vect2 &pos) 
	{
		std::string name;
		// setting up bodyDef only
		// must call createBody to set the parent classes members
		b2FixtureDef fd;
		fd.restitution = .002;
		fd.density = 1.0; // very light
		fd.friction = 7.0;
		fd.isMagnetic = true;
		
		
		//Sprite* sprite = new Sprite("test:cow");
		//this->AddChild(*sprite);
		
		b2CircleShape circleShape;
		
		//Figure out name
		switch (letter)
		{
			case kLetterG:
				name = "menu:G";
				break;
			case kLetterR:
				name = "menu:R";
				break;
			case kLetterO:
				name = "menu:O";
				break;
			case kLetterU:
				name = "menu:U";
				break;
			case kLetterN:
				name = "menu:N";
				break;
			case kLetterD:
				name = "menu:D";
				break;
			case kLetterP:
				name = "menu:P";
				break;
			case kPlay:
				name = "menu:play";
				break;
		}
		
		MenuObject * obj = new MenuObject(name);
		obj->scale = Vect2(.6f,.6f);
		obj->m_soundEffect = "SFX03";
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(pos.x, pos.y);
		
		obj->mp_physicsBody = world.CreateBody(&bodyDef);
		
		float hx = obj->size.x * .6f * .5f;
		float hy = obj->size.y * .6f * .5f;
		
		b2MassData massData;
		massData.center = b2Vec2(0,0);
		massData.mass = 5;
		massData.I = 1.0;
		
		switch (letter)
		{
			case kPlay:
			{
				b2PolygonShape polyShape;
				polyShape.SetAsBox(hx, hy);
				fd.shape = &polyShape;
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&fd);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				obj->mp_fixture->SetIsMagnetic(true);
			}
				break;
			case kLetterG:
			{
				b2PolygonShape polyShape;
				polyShape.SetAsBox( hx * .45f, hy * .75f, b2Vec2(hx * .5f, (hy * .75f * .25f)), 0);
				fd.shape = &polyShape;
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&fd);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				b2CircleShape circleShape;
				circleShape.m_radius = hy;

				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&circleShape, 0.05f);
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				obj->mp_physicsBody->SetMassData(&massData);
				
			}
				break;
			case kLetterU:
			{
				b2CircleShape circleShape;
				circleShape.m_radius = hx;
				circleShape.m_p = b2Vec2(0,hy - hx);
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&circleShape, 0.05f);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				//obj->mp_fixture->SetIsMagnetic(true);
				b2PolygonShape polyShape;
				polyShape.SetAsBox( hx, hy*.5, b2Vec2(0, -hy * .5f), 0);
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&polyShape, 0.05f);	
				//obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				obj->mp_physicsBody->SetMassData(&massData);
			}
				break;
			case kLetterD:
			{
				b2CircleShape circleShape;
				circleShape.m_radius = hx;
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&circleShape, 0.05f);
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				b2PolygonShape polyShape;
				polyShape.SetAsBox( hx * .5f, hy, b2Vec2(-hx * .5f, 0), 0);
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&polyShape, 0.05f);
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				obj->mp_physicsBody->SetMassData(&massData);
			
				
			}
				break;

			case kLetterP:
			{
				b2PolygonShape polyShape;
				polyShape.SetAsBox( hx * .45f, hy, b2Vec2(-hx * .575f, 0), 0);
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&polyShape, 0.05f);
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				b2CircleShape circleShape;
				circleShape.m_radius = hx * .68f;
				circleShape.m_p = b2Vec2(hx * .3f, -hy * .35f);
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&circleShape, 0.05f);	
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				obj->mp_physicsBody->SetMassData(&massData);
			}
				break;
				
			case kLetterR:
			{
				b2PolygonShape polyShape;
				polyShape.SetAsBox( hx * .75f, hy, b2Vec2(-hx * .25f, 0), 0);
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&polyShape, 0.05f);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				b2Vec2 verts[5];
				verts[0].Set(0.0f, 0.0f); //center
				verts[1].Set(hx * .6f, 0.0f); 
				verts[2].Set(hx * .9f, hy * .5f);
				verts[3].Set(hx, hy); //bottom right
				verts[4].Set(0.0f, hy); //bottom middle
				
				polyShape.Set(verts, 5);
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&polyShape, 0.05f);
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				b2CircleShape circleShape;
				circleShape.m_radius = hx * .55f;
				circleShape.m_p = b2Vec2(hx * .25f, -hy * .40f);
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&circleShape, 0.05f);	
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				obj->mp_physicsBody->SetMassData(&massData);
			}
				break;
			case kLetterN:
			{
				b2PolygonShape polyShape;
				polyShape.SetAsBox(obj->size.x/2.0f * .6f, obj->size.y/2.0f * .6f);
				obj->mp_fixture =obj->mp_physicsBody->CreateFixture(&polyShape, .05f);
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				obj->mp_physicsBody->SetMassData(&massData);
			}
				break;
			case kLetterO:
			{
				b2CircleShape circleShape;
				circleShape.m_radius = obj->size.x/2.0f * .6f;
				obj->mp_fixture = obj->mp_physicsBody->CreateFixture(&circleShape, .05f);
				obj->mp_fixture->SetIsMagnetic(true);
				obj->mp_fixture->SetPhysicsSprite(*obj);
				obj->mp_physicsBody->SetMassData(&massData);
			}
				break;

		}
		obj->mp_fixture->SetIsMagnetic(true);
		obj->mp_fixture->SetPhysicsSprite(*obj);
		
		return obj;
	}
	
	MenuObject::MenuObject(const std::string& name) : PhysicsSprite(name)
	{

	}

	MenuObject::~MenuObject()
	{

	}
}
