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
#include "b2DebugDrawer_OGL_ES.h"
#include "Scene.h"

namespace Deadbeef
{

	class MenuScene : public Scene
	{
	public: 
		
		MenuScene(Vect2 avatarPosition);
		virtual ~MenuScene();
		virtual void Update(float elapsedTime);
		bool m_transitioning;
        bool m_touchedDown;
		const virtual std::string MusicName() const { return "MenuMusic"; };
		PhysicsSprite * mp_startButton;
	};
}
