/*
 *  Camera.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/8/10.
 *  Copyright 2010  Andrew Cook. All rights reserved.
 *
 */

#pragma once

#import "Vect2.h"
#import "GameObject.h"
#import "Sprite.h"

namespace Deadbeef
{
	class Camera : public GameObject
	{

		public:  
		Camera(Sprite& targetObject);
		virtual ~Camera();
        virtual void Update(float elapsedTime);
		virtual void Render(float elapsedTime, const Matrix& projection) const;
		
		void SetTarget(Sprite& targetObject);
		void SetPosition(const Vect2& position);
		
		const Vect2& GetPosition() const;
		Vect2 followDistance;
		
		protected:
		Vect2 position;
        Vect2 trackingBoxSize;

		SpriteRef targetSprite;
	};
	typedef std::tr1::shared_ptr<Camera> CameraRef;
}