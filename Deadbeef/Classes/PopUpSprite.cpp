/*
 *  PopUp.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/28/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "PopUpSprite.h"
namespace Deadbeef 
{
	PopUpSprite::PopUpSprite(const std::string imageName, Vect2 position, float fadeInTime):
	Sprite(imageName), m_visible(false), m_fadeTimer(0), m_fadeInTime(fadeInTime)
	{
		this->position = position;
	}

	PopUpSprite::~PopUpSprite()
	{
		
	}
	
	bool PopUpSprite::touchHit(Vect2 touch, Sprite * inSprite, CameraRef cam)
	{
		//printf("image:%f, %f\n", inSprite->position.x, inSprite->position.y);
		//printf("touch: %f, %f\n", touch.x, touch.y);
		float yBoundMax = inSprite->position.y - cam->GetPosition().y + (inSprite->size.y *.5);
		float yBoundMin = inSprite->position.y - cam->GetPosition().y - (inSprite->size.y *.5);
		float xBoundMax = inSprite->position.x - cam->GetPosition().x + (inSprite->size.x *.5);
		float xBoundMin = inSprite->position.x - cam->GetPosition().x - (inSprite->size.x *.5);
		//printf("y boundaries: touch < %f, touch > %f\n", yBoundMax, yBoundMin);
		//printf("x boundaries: touch < %f, touch > %f\n", xBoundMax, xBoundMin);
		
		if (touch.x < xBoundMax && touch.x > xBoundMin)
		{
			
			if(touch.y < yBoundMax && touch.y > yBoundMin)
			{	
				printf("within y constraint\n");
				return true;
			}
			else 
			{
				//printf("not within y const\n");
			}

		}
		else 
		{
			//printf("not inside x and y constraint\n");
		}

		return false;
	}
	void PopUpSprite::Update(float elapsedTime)
	{
		if(m_visible)
		{
			if (m_fadeTimer < m_fadeInTime) 
			{
				m_fadeTimer += elapsedTime;
			}
		}
		else
		{
			if (m_fadeTimer > 0) 
			{
				m_fadeTimer -= elapsedTime;
			}
		}
		
		Sprite::alpha = m_fadeTimer / m_fadeInTime;
		
		Sprite::Update(elapsedTime);
	}


	void PopUpSprite::Show()
	{
		m_visible = true;
	}

	void PopUpSprite::Hide()
	{
		m_visible = false;
	}
}
