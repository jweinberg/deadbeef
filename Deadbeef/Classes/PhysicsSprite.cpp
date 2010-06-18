/*
 *  PhysicsSprite.cpp
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 5/1/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "PhysicsSprite.h"
#include "SoundManager.h"

namespace Deadbeef
{
	PhysicsSprite::PhysicsSprite(const std::string& spriteName): Sprite(spriteName), m_soundEffect("SFX03"), antiGravApplied(false), isMagnetic(false)
	{
		//m_soundEffect = "SFX03";
	}
	// constructor builds box around sprite height and width
	PhysicsSprite::PhysicsSprite(const std::string& spriteName, b2World& world, const Vect2& pos )
						: Sprite(spriteName), m_soundEffect("SFX03"), antiGravApplied(false), isMagnetic(false)
	{
		
		mp_world = &world;
		
		// make body
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(pos.x, pos.y);
		mp_physicsBody = mp_world->CreateBody(&bodyDef);
		
	
		// attach shape to fixture
		b2PolygonShape boxShape;
		boxShape.SetAsBox(this->size.x / 2.0f, this->size.y / 2.0f);
		this->position.x = mp_physicsBody->GetPosition().x;
		this->position.y = mp_physicsBody->GetPosition().y;

		this->rotation = mp_physicsBody->GetAngle() * 180 / M_PI;
		
	
		debug_log("box body size (%f, %f)\n", this->size.x, this->size.y);
		debug_log("PhysicsSprite: texture location (%f, %f)\n", this->position.x,this->position.y);
		//mp_fixture = mp_physicsBody->CreateFixture(&boxShape, 0.2f);// (shape, density)
		
		// fixure definition for describing all attributes assigned to this object
		b2FixtureDef fd;
		fd.restitution = .01; // 1.0 very bouncy =]
		fd.density = 10.0f;
		fd.shape = &boxShape;
		fd.friction = 4.5;
		fd.isMagnetic = true;
		
		b2MassData mass;
		mass.mass = 3.0;
		mass.I = 1;
		mass.center = b2Vec2(0, 0);
		
		
		mp_fixture = mp_physicsBody->CreateFixture(&fd);
		mp_physicsBody->SetMassData(&mass);
		mp_fixture->SetPhysicsSprite(*this);
	}
	// this constructor allows you to buiild the fixture definition outside and pass it in with the shape type
	PhysicsSprite::PhysicsSprite(const std::string& spriteName, b2World& world, const Vect2& pos , b2FixtureDef& fixDef, body_type kBodyType)
						: Sprite(spriteName), m_soundEffect("SFX03"), antiGravApplied(false), isMagnetic(false)
	{
		if (kBodyType == CIRCLE)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(pos.x, pos.y);
			
			mp_physicsBody = world.CreateBody(&bodyDef);
			
			b2CircleShape circleShape;
			circleShape.m_radius = this->size.x/2.0f;
			fixDef.shape = &circleShape;
		}
		else if(kBodyType == BOX )
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(pos.x, pos.y);
			
			mp_physicsBody = world.CreateBody(&bodyDef);
			
			b2PolygonShape boxShape;
			boxShape.SetAsBox(this->size.x/2.0f, this->size.y/2.0f);
			fixDef.shape = &boxShape;
					
		}
		else if(kBodyType == POLYGON)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(pos.x, pos.y);
			
			mp_physicsBody = world.CreateBody(&bodyDef);
			
			int32 count = 6;
			b2Vec2 verts[count];
			int32 theta = (360/ count) *(M_PI / 180);
			float dist = this->size.x/2.0f;
			
			for (int i = 0; i< count; ++i)
			{
				Vect2 temp(cos((float)i * (theta)), sin((float)i * (theta)));
				temp = temp * dist;
				debug_log("%f,%f\n", temp.x, temp.y);
				verts[i].Set(temp.x, temp.y);
			}
			
			b2PolygonShape polyShape;
			polyShape.Set(verts, count);
			fixDef.shape = &polyShape;
			
		}
		this->position.x = mp_physicsBody->GetPosition().x;
		this->position.y = mp_physicsBody->GetPosition().y;
		this->rotation = mp_physicsBody->GetAngle() * 180 / M_PI;	
		
		mp_fixture = mp_physicsBody->CreateFixture(&fixDef);
		mp_fixture->SetPhysicsSprite(*this);
	}
	
	PhysicsSprite::~PhysicsSprite()
	{
		debug_log("physicsSprite Destructor begin:\n");
		// destroying the body automagically destroys joints/shapes
		// p55 paragraph 3
		
		//b2World* current = m_physicsBody->GetWorld();
		//current->DestroyBody(this->m_physicsBody);
		
		mp_physicsBody->GetWorld()->DestroyBody(this->mp_physicsBody);
		mp_fixture = 0;
		mp_physicsBody = 0;
		
		debug_log("physicsSprite Destructor end:\n");
		
	}
	void PhysicsSprite::SetBodyAndFixture(b2World& world, b2FixtureDef& fixDef, b2BodyDef& bodyDef)
	{
		mp_physicsBody = world.CreateBody(&bodyDef);
		mp_fixture = mp_physicsBody->CreateFixture(&fixDef);
		mp_fixture->SetPhysicsSprite(*this);
	}
	void PhysicsSprite::SetAntiGrav()
	{
		if(IsMagnetic())
		{
			if (antiGravApplied == false)
			{
				antiGravApplied = true;
				const b2Vec2 antiGrav = b2Vec2(0, -10 * mp_physicsBody->GetMass());
				mp_physicsBody->ApplyForce(antiGrav, mp_physicsBody->GetPosition());
			}
			else 
			{
				mp_physicsBody->ClearForces();
			}

			
		}
	}
	
	void PhysicsSprite::PlaySound(float volume, bool loop)
	{
		//make sure the sound effect is initialized
		if (m_soundEffect.size() > 0) 
		{
			SoundManager::GetInstance().PlaySound(m_soundEffect, volume, loop);
		}
	}
	void PhysicsSprite::Update(float elapsedTime)
	{
		this->position.x = mp_physicsBody->GetPosition().x;
		this->position.y = mp_physicsBody->GetPosition().y;
		this->rotation = mp_physicsBody->GetAngle() * 180.0f / M_PI;
		//debug_log("PhysicsSprite: texture location (%f, %f)\n", this->position.x,this->position.y);
        
        Sprite::Update(elapsedTime);
		
	}
	void PhysicsSprite::Render(float elapsedTime, const Matrix& projection) const
	{
		Sprite::Render(elapsedTime, projection);
	}
};