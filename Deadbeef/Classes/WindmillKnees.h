/*
 *  WindmillKnees.h
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
	class WindmillPuzzle;
	
	class WindmillKnees: public PuzzlePiece 
	{
		static const float xLock = 16.1;
		static const float yLock = 17.6;
	public:
		WindmillKnees(b2World& world, Vect2 position, Puzzle* parent);
		virtual ~WindmillKnees();
		
		void Update(float elapsedTime);
		
		b2Fixture* mp_sensorBottomLeft;
		b2Fixture* mp_sensorBottomRight;
		b2Fixture* mp_sensorTopCenter;
	};
}