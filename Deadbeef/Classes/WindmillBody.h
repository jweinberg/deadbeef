/*
 *  WindmillBody.h
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
	class WindmillBody: public PuzzlePiece 
	{
		static const float xLock = 16.1;
		static const float yLock = 13.45;
		
	public:
		WindmillBody(b2World& world, Vect2 position, Puzzle* parent);
		virtual ~WindmillBody();
		
		void Update(float elapsedTime);
		
		b2Fixture* mp_sensorBottom;
		b2Fixture* mp_sensorTop;
	};
}