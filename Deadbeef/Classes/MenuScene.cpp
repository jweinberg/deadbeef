/*
 *  MenuScene.cpp
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 4/30/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "MenuScene.h"
#include "TouchManager.h"
#include "Matrix.h"
#include "Vect2.h"
#include "PhysicsSprite.h"
#include "ParticleEmitter.h"
#include "SoundManager.h"
#include "Magnet.h"
#include "MenuObject.h"
#include "SceneController.h"

namespace Deadbeef
{
	MenuScene::MenuScene(Vect2 avatarPosition) : Scene(avatarPosition), m_transitioning(false), m_touchedDown(false)
	{
		// make left Wall
		b2BodyDef leftWallBody;
		leftWallBody.position.Set(-1.0f,0.0);
		
		b2PolygonShape wallShape;
		wallShape.SetAsBox(.5f, 26.0f);
		mp_physicsWorld->CreateBody(&leftWallBody)->CreateFixture(&wallShape, 0.0f);
		
		// make right Wall
		b2BodyDef rightWallBody;
		rightWallBody.position.Set(33.0f,0.0);
		mp_physicsWorld->CreateBody(&rightWallBody)->CreateFixture(&wallShape, 0.0f);		
		
		b2FixtureDef fixDef;
		fixDef.density = 1.0f;
		fixDef.friction = 0.3f;
		
		MenuObject * gObj = MenuObject::CreateMenuLetter(kLetterG, *this->mp_physicsWorld, Vect2(4.0,20));
		mp_startButton = MenuObject::CreateMenuLetter(kPlay, *this->mp_physicsWorld, Vect2(16,15));
		MenuObject * rObj = MenuObject::CreateMenuLetter(kLetterR, *this->mp_physicsWorld, Vect2(8,16));
		MenuObject * oObj = MenuObject::CreateMenuLetter(kLetterO, *this->mp_physicsWorld, Vect2(11,20));
		MenuObject * uObj = MenuObject::CreateMenuLetter(kLetterU, *this->mp_physicsWorld, Vect2(15,20));
		MenuObject * nObj = MenuObject::CreateMenuLetter(kLetterN, *this->mp_physicsWorld, Vect2(19,16));
		MenuObject * dObj = MenuObject::CreateMenuLetter(kLetterD, *this->mp_physicsWorld, Vect2(23,20));

		MenuObject * u2Obj = MenuObject::CreateMenuLetter(kLetterU, *this->mp_physicsWorld, Vect2(26,18));
		MenuObject * pObj = MenuObject::CreateMenuLetter(kLetterP, *this->mp_physicsWorld, Vect2(30,15));
		
		mr_camera->AddChild(*gObj);
		mr_camera->AddChild(*rObj);
		mr_camera->AddChild(*oObj);
		mr_camera->AddChild(*uObj);
		mr_camera->AddChild(*nObj);
		mr_camera->AddChild(*dObj);
		
		mr_camera->AddChild(*u2Obj);		
		mr_camera->AddChild(*pObj);
		mr_camera->AddChild(*mp_startButton);
		mr_camera->followDistance = Vect2(16,12);
	}
	
	MenuScene::~MenuScene()
	{
	}
	
	void MenuScene::Update(float elapsedTime)
	{
		const TouchList& touches = TouchManager::GetInstance().Touches();
		
		if (!m_transitioning && touches.size() == 1)
		{
			TouchRef touch = touches[0];
			if ((!m_touchedDown && (touch->state == kTouchBegan)) ||
               (m_touchedDown && (touch->state == kTouchEnded || touch->state == kTouchCancelled)))
			{
				b2Vec2 screenCoord = b2Vec2(touch->x/32.0f, touch->y/32.0f);
				screenCoord += b2Vec2(mr_camera->GetPosition().x, mr_camera->GetPosition().y);
				
				bool touched = mp_startButton->mp_fixture->TestPoint(screenCoord);
				if (touched)
				{
                    if (m_touchedDown)
                    {
                        mp_controller->TransitionToScene(kScene_Game);
                        m_transitioning = true;
                        mp_startButton->SetImage("menu:play");
                    }
                    else
                    {
                        m_touchedDown = true;
                        mp_startButton->SetImage("menu:playpressed");
                    }
				}
                else
                {
                    m_touchedDown = false;
                    mp_startButton->SetImage("menu:play");
                }
                
			}			
		}
		
		Scene::Update(elapsedTime);
	}
}
