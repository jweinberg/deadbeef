/*
 *  GameScene.h
 *  Deadbeef
 *
 *  Created by Brad Helgeland on 4/30/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#pragma once

#include "GameObject.h"
#include "Box2D.h"
#include "Sprite.h"
#include "ParallaxSprite.h"
#include "PhysicsSprite.h"
#include "Camera.h"
#include "Avatar.h"
#include "WindmillPuzzle.h"
#include "CloudParticles.h"
#include "ParticleEmitter.h"
#include "b2DebugDrawer_OGL_ES.h"
#include "ContactListener.h"
#include "Scene.h"

namespace Deadbeef
{
	class GameScene : public Scene
	{
	public: 
		
		GameScene(Vect2 avatarPos);
		virtual ~GameScene();
		void Update(float elapsedTime);
		
	protected:
		
		WindmillPuzzle * mp_windmillPuzzle;
        float m_timeBeforeFinish;
        bool m_transitioning;
		bool m_startedCountdown;
		
		const virtual std::string MusicName() const { return "GameMusic"; };
	};
}
