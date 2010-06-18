/*
 *  GameScene.cpp
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 4/30/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "GameScene.h"
#include "TouchManager.h"
#include "Matrix.h"
#include "Vect2.h"
#include "PhysicsSprite.h"
#include "ParticleEmitter.h"
#include "SoundManager.h"
#include "Magnet.h"
#include "b2DebugDrawer_OGL_ES.h"
#include "PhysicsSprite.h"
#include "SceneController.h"

namespace Deadbeef
{
	GameScene::GameScene(Vect2 avatarPosition) : Scene(avatarPosition), m_startedCountdown(false), m_transitioning(false)
	{
		b2FixtureDef fixDef;
		fixDef.density = 1.0f;
		fixDef.friction = 0.3f;
		
		// make left wall
		b2BodyDef leftWallBody;
		leftWallBody.position.Set(-30.0f,0.0);
		
		b2PolygonShape wallShape;
		wallShape.SetAsBox(.5f, 26.0f);
		mp_physicsWorld->CreateBody(&leftWallBody)->CreateFixture(&wallShape, 0.0f);
		
		b2PolygonShape ceilingShape;
		ceilingShape.SetAsBox(50.0f, .5f);
		
		// make right Wall
		b2BodyDef rightWallBody;
		rightWallBody.position.Set(50.0f,0.0);
		mp_physicsWorld->CreateBody(&rightWallBody)->CreateFixture(&wallShape, 0.0f);	
		
		b2BodyDef ceilingBody;
		ceilingBody.position.Set(0.0f, -20.0f);
		mp_physicsWorld->CreateBody(&ceilingBody)->CreateFixture(&ceilingShape, 0.0f);
		
		mp_windmillPuzzle = new WindmillPuzzle(mp_physicsWorld);
		
		PhysicsSprite* junk[6];
		junk[0] = new PhysicsSprite("Junk:bigpipe", *mp_physicsWorld, Vect2(-8, 15));
		junk[1] = new PhysicsSprite("Junk:junkrect", *mp_physicsWorld, Vect2(1, 15));
		junk[2] = new PhysicsSprite("Junk:bigpipe", *mp_physicsWorld, Vect2(10, 15));
		junk[3] = new PhysicsSprite("Junk:smallpipe", *mp_physicsWorld, Vect2(15, 15));
		junk[4] = new PhysicsSprite("Junk:bigpipe", *mp_physicsWorld, Vect2(20, 15));
		junk[5] = new PhysicsSprite("Junk:junkrect", *mp_physicsWorld, Vect2(25, 15));
		
		//Sprite * blueprintSprite = new Sprite("test:cow");
		//blueprintSprite->position.x = 16.0;
		//blueprintSprite->position.y = 12.0;
		
		for (int i = 0; i < 6; ++i) 
		{
			mr_camera->AddChild(*junk[i]);
		}
		
		mr_camera->AddChild(*mp_windmillPuzzle);
		//camera->AddChild(*blueprintSprite);
	}
	
	GameScene::~GameScene()
	{	
	}

	    
    void GameScene::Update(float elapsedTime)
    {
        if (!m_transitioning)
        {
            //Start the countdown
            if (!m_startedCountdown && mp_windmillPuzzle->IsComplete())
            {
                m_startedCountdown = true;
                m_timeBeforeFinish = 4.0f;
            }
            else if (m_startedCountdown)
            {
                m_timeBeforeFinish -= elapsedTime;
                if (m_timeBeforeFinish < 0)
                {
                    m_startedCountdown = false;
                    m_transitioning = true;
                    mp_controller->TransitionToScene(kScene_Credits);
                }
            }
        }
        Scene::Update(elapsedTime);
    }

}
