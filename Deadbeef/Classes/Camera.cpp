/*
 *  Camera.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/8/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "Camera.h"

namespace Deadbeef
{
	Camera::Camera(Sprite& targetSprite): position(0.0, 0.0), followDistance(5.0, 5.0)
	{
		this->targetSprite= SpriteRef(&targetSprite);
	}
	
	Camera::~Camera()	
	{

	}
	
	void Camera::SetTarget(Sprite& targetSprite)
	{
		this->targetSprite= SpriteRef(&targetSprite);
	}
	
	void Camera::SetPosition(const Vect2& position)
	{
		this->position = position;
	}
	
	const Vect2& Camera::GetPosition() const
	{
		return this->position;
	}
	
	void Camera::Update(float elapsedTime)
	{
		
		//Modify X Posiiton
		float xDist = targetSprite->position.x - (position.x + 16);
		
		if(xDist < -followDistance.x)
		{
			position.x += xDist + followDistance.x;
			debug_log("Camera Postion: %f, %f \n", position.x, position.y);
		}
		else if(xDist > followDistance.x)
		{
			position.x += xDist - followDistance.x;
			debug_log("Camera Postion: %f, %f \n", position.x, position.y);
		}
		
		//Modify Y Position
		float yDist = targetSprite->position.y - (position.y);
		
		if(yDist < followDistance.y)
		{
			position.y -= followDistance.y - yDist;
			debug_log("Camera Postion: %f, %f \n", position.x, position.y);
		}
		else if(yDist > followDistance.y)
		{
			position.y -= followDistance.y - yDist;
			debug_log("Camera Postion: %f, %f \n", position.x, position.y);
		}
		
		if (position.x < -30) 
		{
			position.x = -30;
		}
		else if(position.x > 18)
		{
			position.x = 18;
		}
		
		if (position.y > 0) 
		{
			position.y = 0;
		}
		else if(position.y < -12)
		{
			position.y = -12;
		}
			
		
		GameObject::Update(elapsedTime);
	}
	
	void Camera::Render(float elapsedTime, const Matrix& projection) const
	{
		Matrix translations = Matrix::MakeTranslation(-position.x, -position.y, 0);
		
		translations =  translations * projection;
		
		GameObject::Render(elapsedTime, translations);
	}
}

