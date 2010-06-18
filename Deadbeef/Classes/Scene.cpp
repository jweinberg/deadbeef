/*
 *  Scene.cpp
 *  Deadbeef
 *
 *  Created by Devon on 4/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"
#include "SceneController.h"
#include "TouchManager.h"
#include "ParticleSpawnDefinition.h"
#include "CloudParticles.h"

const float PINCH_CONSTRAINT = .85f;
const float PINCH_HIGH = 1.5f;

namespace Deadbeef
{
	
	Scene::Scene(Vect2 avatarPosition) : GameObject() , m_acceptInput(true), m_outOfBoundsTimer(0),
	m_infoButtonTimer(0)
	{
		m_timeStep = 1.0f/60.0f;
		m_velIterations = 10;
		m_posIterations = 10;

		// setup initial world attributes
		b2Vec2 gravity;
		gravity.Set(0.0f, 10.0f);
		bool doSleep = true;
		mp_physicsWorld = new b2World(gravity, doSleep);
		mp_physicsWorld->SetAutoClearForces(false);
		
		m_blueprintIsActive = false;
		m_touchedDown = false;
		
		mp_contactListener = new Deadbeef::ContactListener();
		mp_physicsWorld->SetContactListener(mp_contactListener);
		
	#ifdef DEBUG
		mp_debugDraw = new b2DebugDrawer_OGL_ES();
		mp_debugDraw->SetFlags(b2DebugDraw::e_shapeBit | b2DebugDraw::e_jointBit | b2DebugDraw::e_aabbBit);// | b2DebugDraw::e_pairBit | b2DebugDraw::e_centerOfMassBit);
		mp_physicsWorld->SetDebugDraw(mp_debugDraw);
	#endif
		//setup Avatar
		m_Avatar = new Avatar(*mp_physicsWorld, avatarPosition); 

		// make ground body
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, 24.0f);
		mp_groundBody = mp_physicsWorld->CreateBody(&groundBodyDef);
		
		// attach ground fixture
		b2PolygonShape groundShape;
		groundShape.SetAsBox(128.0f, 1.0f);
		mp_groundBody->CreateFixture(&groundShape, 0.0f);
		
        // make ceiling body
		b2BodyDef skyBodyDef;
		skyBodyDef.position.Set(0.0f, -20.0f);
		mp_skyBody = mp_physicsWorld->CreateBody(&skyBodyDef);
		
		// attach ground fixture
        b2PolygonShape skyShape;
		skyShape.SetAsBox(128.0f, 1.0f);

		mp_skyBody->CreateFixture(&skyShape, 0.0f);
		
		
		Camera* newCamera = new Camera(*m_Avatar);
		mr_camera = CameraRef(newCamera);
		
		mp_cloudEmitter = new ParticleEmitter(Vect2(0, 0), 100, SpawnDefinitionRef(new CloudParticles(mr_camera)));
        
		mr_missionBrief = PopUpSpriteRef(new PopUpSprite("MissionBrief:MissionBrief", Vect2(16, 12), 1.0));
		
		// setup blueprint button
		mp_bluePrintButton = new Sprite("menu:InfoButton");
		mp_bluePrintButton->position.x = mr_camera->GetPosition().x + 32 - mp_bluePrintButton->size.x *.75;
		mp_bluePrintButton->position.y = mr_camera->GetPosition().y + 0 + mp_bluePrintButton->size.x *.75;
		//printf("%f, %f\n", mp_bluePrintButton->position.x, mp_bluePrintButton->position.y);
		
		
		mr_camera->AddChild(*m_Avatar);
		mr_camera->AddChild(*m_Avatar->GetMagnet());
		mr_camera->AddChild(*mp_cloudEmitter);
		mr_camera->AddChild(*mp_bluePrintButton);
	}

	Scene::~Scene()
	{
			
	}
	void Scene::ActivateBlueprint()
	{
		Sprite * blueprint = new Sprite("test:Origin_Icon");
		blueprint->position.x = mr_camera->GetPosition().x + 16;// middle of screen width
		blueprint->position.y = mr_camera->GetPosition().y + 12;// middle of screen height
		mr_camera->AddChild(*blueprint);
		mp_blueprint = blueprint;
		
	}
	void Scene::DeactivateBlueprint()
	{
		if (mp_blueprint) 
		{
			mr_camera->RemoveChild(*mp_blueprint);
		}
		else 
		{
			
			mp_blueprint = 0;
		}
	}
	void Scene::GetGestures()
	{
		TouchManager& manager = TouchManager::GetInstance();
		
		const TouchList& touches = manager.Touches();
		
		//Single touch
		if (touches.size() == 1)
		{
			TouchRef touch = touches[0];
			
			// gets touches for blueprint button
			float touchConvX = touch->x /32;
			float touchConvY = touch->y /32;
			
			if ((!m_touchedDown && (touch->state == kTouchBegan)) ||
				(m_touchedDown && (touch->state == kTouchEnded || touch->state == kTouchCancelled)))
			{
				bool pressed = mr_missionBrief->touchHit( Vect2(touchConvX, touchConvY), mp_bluePrintButton, mr_camera);
				if (pressed) 
				{
					if(m_touchedDown)
					{
						// activate blueprint
						if(!mr_missionBrief->IsVisible())
						{
							mr_missionBrief->Show();
						}
						else 
						{
							mr_missionBrief->Hide();
						}
					
						m_infoButtonTimer = 0;
					}
					else 
					{
						m_touchedDown = true;
					}

				}
				else 
				{
					m_touchedDown = false;
				}
			}
			else
			{
				//Figure out vector
				b2Vec2 curPoint(touch->x, touch->y);
				b2Vec2 oldPoint(touch->prevX, touch->prevY);
			
				m_Avatar->MoveAvatar(.15f * (curPoint-oldPoint));
			}			
		}
		
		// PINCH
		if (manager.CurrentPinch().isValid())
		{
			static bool debounce = true;
			float scale = manager.CurrentPinch().getScale();
			if (debounce)
			{
				if (scale < PINCH_CONSTRAINT || scale > PINCH_HIGH)
				{
					debounce = false;
					if (!m_Avatar->IsMagnetOn())
					{
						m_Avatar->ActivateMagnet();
					}
                    else
					{
						m_Avatar->DeactivateMagnet();
					}
				}
			}
			else if(scale >PINCH_CONSTRAINT && scale < PINCH_HIGH)
			{
				debounce = true;
			}
		}
	}

	void Scene::Update(float elapsedTime)
	{
		mr_missionBrief->Update(elapsedTime);
		
		// simulation
		if (m_acceptInput)
		{
			this->GetGestures();
		}
			
		if(m_infoButtonTimer < 1)
		{
			m_infoButtonTimer += elapsedTime;
		}
		
		//Out of bounds check
		if (m_Avatar->GetPosition()->x < -30 ||
			m_Avatar->GetPosition()->x > 46) 
		{
			m_outOfBoundsTimer += elapsedTime;
			
			//Player is out of bounds.  Warn them of impending doom.			
			if(m_outOfBoundsTimer > 8)
			{
				//Avatar has been out of bounds too long do something
			}
		}
		else 
		{
			m_outOfBoundsTimer = 0;
		}

		this->mr_camera->Update(elapsedTime);
		
		// update blueprint button
		mp_bluePrintButton->position.x = mr_camera->GetPosition().x + 32 - mp_bluePrintButton->size.x *.75;
		mp_bluePrintButton->position.y = mr_camera->GetPosition().y + 0 + mp_bluePrintButton->size.x *.75;
		
		if (m_blueprintIsActive && mp_blueprint)
		{
			mp_blueprint->position.x = mr_camera->GetPosition().x + 16;// middle of screen width
			mp_blueprint->position.y = mr_camera->GetPosition().y + 12;// middle of screen height
		}
		// simulation
		mp_physicsWorld->Step(m_timeStep , m_velIterations, m_posIterations);
		
		if(mr_background.get())
		{
			this->mr_background->SetOffset(mr_camera->GetPosition().x / 32, mr_camera->GetPosition().y / 32);
			this->mp_cloudEmitter->m_origin = mr_camera->GetPosition();
		}
		
		GameObject::Update(elapsedTime);
	}
		
	void Scene::Render(float elapsedTime, const Matrix& projection) const
	{
		if(mr_background.get())
		{
			mr_background->Render(elapsedTime, projection);
		}
		
		mr_camera->Render(elapsedTime, projection);
		
		mr_missionBrief->Render(elapsedTime, projection);
		
        GameObject::Render(elapsedTime, projection);
		

		
#ifdef DEBUG
		Matrix translations = Matrix::MakeTranslation(-mr_camera->GetPosition().x, -mr_camera->GetPosition().y, 0);
		mp_debugDraw->SetProjection(translations * projection);
		mp_physicsWorld->DrawDebugData();
#endif
	}
}

