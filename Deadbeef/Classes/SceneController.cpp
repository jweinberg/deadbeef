/*
 *  SceneController.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SceneController.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "SoundManager.h"

namespace Deadbeef
{
	SceneController::SceneController(SceneRef scene) : mr_currentScene(scene)
	{
		mr_currentScene->mp_controller = this;
        for (int i = 0; i < 1000; ++i)
        {
            mr_currentScene->mp_cloudEmitter->Update(.1f);
        }
		m_sceneOffset = 0.0f;
		m_transitioning = false;
		mr_backgroundMusic = SoundManager::GetInstance().PlaySound(scene->MusicName(), 1.0f, true);
	}

	SceneController::~SceneController()
	{
	}
    
#define MIN(A,B) ((A)<(B)?(A):(B))
		
	void SceneController::Update(float elapsedTime)
	{
		if (m_transitioning)
		{
			switch (mk_transState)
			{
				case kTransitionState_StartFloatUp:
				{
					mr_currentScene->m_Avatar->MoveToTarget(b2Vec2(16.0f, -20.0f));
					mk_transState = kTransitionState_FloatUp;
                    mr_currentScene->mr_camera->followDistance = Vect2(16,12);
				}
					break;
				case kTransitionState_FloatUp:
				{
					Avatar * avatar = mr_currentScene->m_Avatar;
                    m_transitionTimer += elapsedTime;
					if (avatar->mp_physicsBody->GetPosition().y < -13.0f || m_transitionTimer > 4.0f)
					{
                        if (mk_transitionTo == kScene_Credits)
                            mk_transState = kTransitionState_ShowCredits;
                        else
                            mk_transState = kTransitionState_ReloadParts;
                        
					}					
				}
					break;
                case kTransitionState_ShowCredits:
                {
                    m_creditsAlpha += elapsedTime * .3f;
                    m_creditsAlpha = MIN(m_creditsAlpha, 1.0f);
                    mr_credits->alpha = m_creditsAlpha;
                    m_creditsTimer += elapsedTime;
                    if (m_creditsTimer > 8.0f)
                    {
                        mk_transState = kTransitionState_ReloadParts;
                    }
                }
                    break;
				case kTransitionState_ReloadParts:
				{
					b2Vec2 pos = mr_currentScene->m_Avatar->mp_physicsBody->GetPosition();
					SceneRef newScene;
                    
                    switch (mk_transitionTo)
                    {
                        case kScene_Game:
                            newScene = SceneRef(new GameScene(Vect2(pos.x, -15.0f)));
                            break;
                        case kScene_Credits:
                        case kScene_Menu:
                            newScene = SceneRef(new MenuScene(Vect2(pos.x, -15.0f)));
                            break;
                    }
					newScene->mr_background = mr_currentScene->mr_background;
					GameObjectRef particles = mr_currentScene->mr_camera->FindChild(*mr_currentScene->mp_cloudEmitter);
					newScene->mr_camera->AddChild(particles);
					newScene->mr_camera->SetPosition(Vect2(0,0));
					newScene->mp_controller = this;
					mr_currentScene = newScene;
                    if (mk_transitionTo == kScene_Credits)
                        mk_transState = kTransitionState_ShowCreditsOut;
                    else
                        m_transitioning = false;
				}
					break;
                case kTransitionState_ShowCreditsOut:
                {
                    m_creditsAlpha -= elapsedTime * .3f;
                    mr_credits->alpha = m_creditsAlpha;
                    if (m_creditsAlpha < 0.0f)
                        m_transitioning = false;
                    
                }
                    break;
				case kTransitionState_FloatDown:
					break;
			}
		}

		mr_currentScene->Update(elapsedTime);		
	}
	
	void SceneController::Render(float elapsedTime, const Matrix& projection) const
	{
		mr_currentScene->Render(elapsedTime, projection);
        if (m_transitioning && mk_transitionTo == kScene_Credits)
        { 
            if (mr_credits.get())
                mr_credits->Render(elapsedTime, projection);
        }
	}
	
	void SceneController::TransitionToScene(kScene scene)
	{
		if (mr_backgroundMusic.get())
			mr_backgroundMusic->Stop();
        mk_transitionTo = scene;
        m_transitionTimer = 0.0f;
        switch (mk_transitionTo) 
        {
            case kScene_Credits:
                mr_credits = SpriteRef(new Sprite("Credits:credits")); //intentinal fallthrough
                mr_credits->position = Vect2(16,12);
                mr_credits->alpha = 0.0f;
                m_creditsAlpha = 0.0f;
                m_creditsTimer = 0.0f;
            case kScene_Menu:
                mr_backgroundMusic = SoundManager::GetInstance().PlaySound("MenuMusic", 1.0f, true);
                break;
            case kScene_Game:
                mr_backgroundMusic = SoundManager::GetInstance().PlaySound("GameMusic", 1.0f, true);
                break;
        }
		
		mr_currentScene->m_Avatar->DeactivateMagnet();
		mr_currentScene->SetAcceptsInput(false);
		m_transitioning = true;
		mk_transState = kTransitionState_StartFloatUp;
	}
    
	void SceneController::AddChild(GameObject& child)
	{
		//This object is sterile...No children allowed
	}
	
	void SceneController::RemoveChild(GameObject& child)
	{
		
		//This object is sterile...No children allowed
	}	
	
	
	
}