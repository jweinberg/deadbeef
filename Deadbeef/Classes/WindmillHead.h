/*
 *  WindmillHead.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/20/10.
 *  Copyright 2010  . All rights reserved.
 *
 */


#pragma once

#include "PuzzlePiece.h"	

namespace Deadbeef 
{	
	class WindmillHead: public PuzzlePiece 
	{
		static const float xLock = 16.1;
		static const float yLock = 8.75;
		
	public:
		WindmillHead(b2World& world, Vect2 position, Puzzle* parent);
		virtual ~WindmillHead();
		
		void Update(float elapsedTime);
		void Render(float elapsedTime, const Matrix& projection) const;
		b2Fixture* mp_sensorBottom;
		
		Deadbeef::Sprite* bladeSprite; 
		
	};
}