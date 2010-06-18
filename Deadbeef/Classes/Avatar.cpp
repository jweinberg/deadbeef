/*
 *  Avatar.cpp
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 5/8/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "Avatar.h"
#include "Box2D.h"
#include "Sprite.h"
#include "TouchManager.h"
#include "ParticleSpawnDefinition.h"
#include "SoundManager.h"



// counteract Gravity
const float ANTI_GRAV = -1.15; // y direction only

// bring the balloon back toward the center of it's desired position
const float RETURN_VEL = -1.2; // scale amount for a negative velocity

namespace Deadbeef
{
	Avatar::Avatar(b2World& world, Vect2 position) : PhysicsSprite("Balloon:base")//, world, position)
	{
		depth = 10;
		m_movingToTarget = false;
		m_magnetOn = false;
		
		// setting up bodyDef only
		// must call createBody to set the parent classes members
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(position.x, position.y);
		bodyDef.angularDamping = 5000000;
	
		// setting up bodyDef only
		// must call createBody to set the parent classes members
		b2FixtureDef fd;
		fd.restitution = .002;
		fd.density = 5.0; // very light
		fd.friction = 7.0;
		fd.isMagnetic = false;
		
		b2PolygonShape polyShape;
		polyShape.SetAsBox(this->size.x/2.0f, this->size.y/2.0f);
		fd.shape = &polyShape;
		
		Deadbeef::PhysicsSprite::SetBodyAndFixture(world, fd, bodyDef);
		
		const b2Vec2 antiGrav = b2Vec2(0, -10 * mp_physicsBody->GetMass());
		
		// create magnet

		Vect2 magPosition(position.x, position.y + 1.5);

		mp_Magnet = new Deadbeef::Magnet(world, magPosition, 255.0f, 8.0f);
		mp_Magnet->Disable();

		
		// revolute joint
		b2RevoluteJointDef revJointDef;
		
		//b2Body * prevBody = mp_physicsBody;
		revJointDef.lowerAngle = -.125f * b2_pi;
		revJointDef.upperAngle = .125f * b2_pi;
		revJointDef.enableLimit = true;
		revJointDef.collideConnected = false;
		
		revJointDef.Initialize(mp_physicsBody, mp_Magnet->GetBody(), mp_physicsBody->GetWorldCenter());
		world.CreateJoint(&revJointDef);
		
        b2CircleShape circle;
        circle.m_radius = 1.125f;
        circle.m_p = b2Vec2(0, -3.0f);
        fd.shape = &circle;
        
        mp_physicsBody->CreateFixture(&fd);
        
		b2MassData newMass;
		newMass.mass = 5.0f;
		newMass.I = 1.0;
		newMass.center = b2Vec2(0, 0);
		
		mp_physicsBody->SetMassData(&newMass);
		mp_physicsBody->ApplyForce(antiGrav, mp_physicsBody->GetPosition());		
		this->position = position;
		Sprite * balloon = new Sprite("Balloon:balloon");
		balloon->frameRate = 6;
		balloon->position = Vect2(0,-2.25f);
		this->AddChild(*balloon);
		
		Sprite * leftFan = new Sprite("Balloon:leftfan");
		leftFan->position = Vect2(-0.84375, 0.1875);
		this->AddChild(*leftFan);
		
		Sprite * rightFan = new Sprite("Balloon:rightfan");
		rightFan->position = Vect2(0.84375, 0.1875);
		this->AddChild(*rightFan);
		
		Sprite * flame = new Sprite("Balloon:flame");
		flame->frameRate = 6.0f;
		flame->position = Vect2(-.05f, -.64f);
		this->AddChild(*flame);
		
		//this->m_soundEffect = "SFX03";
	}
	
	Avatar::~Avatar()
	{
		// parent class destructor called automagically
	}
	
	void Avatar::MoveToTarget(b2Vec2 target)
	{
		m_desiredPosition = target;
		m_movingToTarget = true;
	}
	
	void Avatar::Update(float elapsedTime)
	{				
		if (!m_movingToTarget)
		{
			/* This code is used to stablized the balloon in the center of the screen */
			//float pressureDelta = (mp_physicsBody->GetPosition().y) * 0.01;			
			//mp_physicsBody->ApplyLinearImpulse(b2Vec2(0,-0.14 - pressureDelta), mp_physicsBody->GetPosition());
			
			if (mp_physicsBody->GetPosition().y < -10) 
			{
				mp_physicsBody->ApplyLinearImpulse(b2Vec2(0,0.5), mp_physicsBody->GetPosition());
			}
			float newAngularVelocity = -mp_physicsBody->GetAngle() + mp_physicsBody->GetAngularVelocity();
			mp_physicsBody->SetAngularDamping(2);
			mp_physicsBody->SetAngularVelocity(newAngularVelocity);	
		}
		else
		{
			b2Vec2 direction = (m_desiredPosition - mp_physicsBody->GetPosition());
			direction.Normalize();
			mp_physicsBody->ClearForces();
			mp_physicsBody->ApplyForce(500.0f * direction, mp_physicsBody->GetPosition());
		}

		PhysicsSprite::Update(elapsedTime);
	}

	void Avatar::ActivateMagnet()
	{
		mp_Magnet->Enable();
		m_magnetOn = true;
		SoundManager::GetInstance().PlaySound("SFX01", 3.0, false);
	}
	void Avatar::DeactivateMagnet()
	{
		mp_Magnet->Disable();
		m_magnetOn = false;
		SoundManager::GetInstance().PlaySound("SFX07", 3.0, false);		
	}
	
	void Avatar::MoveAvatar(b2Vec2 impulse)
	{
		mp_physicsBody->ApplyLinearImpulse(impulse, mp_physicsBody->GetPosition());
	}
}

















