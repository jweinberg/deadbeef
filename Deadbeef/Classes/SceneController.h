/*
 *  SceneController.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "Scene.h"
#include "GameObject.h"
#include "SoundManager.h"

namespace Deadbeef
{
	typedef enum
	{
		kTransitionState_StartFloatUp,
		kTransitionState_FloatUp,
		kTransitionState_ReloadParts,
		kTransitionState_FloatDown,
        kTransitionState_ShowCredits,
        kTransitionState_ShowCreditsOut,
	}kTransitionState;
    
    typedef enum
    {
        kScene_Menu,
        kScene_Game,
        kScene_Credits,
    }kScene;
	
	class SceneController : public GameObject
	{
	public:
		SceneController(SceneRef scene);
		virtual ~SceneController();

		void TransitionToScene(kScene scene);
		virtual void Update(float elapsedTime);
		virtual void Render(float elapsedTime, const Matrix& projection) const;
		virtual void AddChild(GameObject& child);
		virtual void RemoveChild(GameObject& child);
		
	private:
		SoundRecordRef mr_backgroundMusic;
		
		kTransitionState mk_transState;
		float m_sceneOffset;
        float m_creditsAlpha;
        float m_creditsTimer;
        float m_transitionTimer;
		bool m_transitioning;
        SpriteRef mr_credits;
        kScene mk_transitionTo;
		SceneRef mr_currentScene;
		SceneRef mr_inScene;
	};
}
