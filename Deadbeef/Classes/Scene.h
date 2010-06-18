/*
 *  Scene.h
 *  Deadbeef
 *
 *  Created by Devon on 4/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "GameoBject.h"
#include "Box2D.h"
#include <tr1/memory>
#include "ParallaxSprite.h"
#include "Avatar.h"
#include "b2DebugDrawer_OGL_ES.h"
#include "ContactListener.h"
#include "Camera.h"
#include "PopUpSprite.h"

namespace Deadbeef
{	
	
	class SceneController;

	class Scene : public GameObject
	{
	public: 
		Scene(Vect2 avatarPosition = Vect2(16.0f,-15.0f));
		virtual ~Scene();
		
		void SetBackground(const ParallaxSpriteRef &mr_background) { this->mr_background = mr_background; }
		
		
		virtual void Update(float elapsedTime);
		virtual void Render(float elapsedTime, const Matrix& projection) const;
		void GetGestures();
		SceneController* GetController(){return mp_controller;};
		void SetAcceptsInput(bool input) { this->m_acceptInput = input; };
		void Pause();
		void Unpause();
		void ActivateBlueprint();
		void DeactivateBlueprint();
		
	protected:
		SceneController * mp_controller;
		bool m_paused;
		float m_infoButtonTimer;
		float m_outOfBoundsTimer;
		b2DebugDrawer_OGL_ES * mp_debugDraw;
		b2World * mp_physicsWorld;
		
		ParallaxSpriteRef mr_background;
		Avatar * m_Avatar;
		
		ContactListener * mp_contactListener;
		float32 m_timeStep;
        b2Body * mp_skyBody;
		b2Body * mp_groundBody;
		b2Body * mp_dynamicBody;
		int32 m_velIterations;
		int32 m_posIterations;
		bool m_acceptInput;
		
		bool m_activateBlueprint;
		bool m_blueprintIsActive;
		bool m_touchedDown;
		
		ParticleEmitter* mp_cloudEmitter;
		CameraRef mr_camera;
		
		PopUpSpriteRef mr_missionBrief;
		Sprite * mp_bluePrintButton;
		Sprite * mp_blueprint;
		
		const virtual std::string MusicName() const = 0;
		
		friend class SceneController;
	};
	
	typedef std::tr1::shared_ptr<Scene> SceneRef;
}