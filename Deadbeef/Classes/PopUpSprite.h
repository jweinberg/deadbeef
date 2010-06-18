/*
 *  PopUp.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/28/10.
 *  Copyright 2010  . All rights reserved.
 *
 */
#include "Sprite.h"
#include "Camera.h"

namespace Deadbeef 
{
	class PopUpSprite : public Sprite 
	{
		
	public:
		PopUpSprite(const std::string imageName, Vect2 position, float fadeInTime);
		virtual ~PopUpSprite();
		
		void Update(float elapsedTime);
		bool touchHit(Vect2	touch, Sprite * inSprite, CameraRef cam);
		void Show();
		void Hide();
		
		bool IsVisible(){return m_visible;}
		
	protected:
		bool m_visible;
		float m_fadeInTime;
		float m_fadeTimer;
		
		
	};
	
	typedef std::tr1::shared_ptr<PopUpSprite> PopUpSpriteRef;
	
}