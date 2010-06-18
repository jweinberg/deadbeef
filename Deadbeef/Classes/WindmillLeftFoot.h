/*
 *  WindmillSquarePiece.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/20/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#pragma once

#include "PuzzlePiece.h"	
#include "Puzzle.h"

namespace Deadbeef 
{	
	class PuzzlePiece;
	
	class WindmillLeftFoot : public PuzzlePiece 
	{
	static const float xLock = 14.35;
	static const float yLock = 20.3;
		
	public:
		WindmillLeftFoot(b2World& world, Vect2 position, Puzzle* parent);
		virtual ~WindmillLeftFoot();
		
		void Update(float elapsedTime);
	
	protected:
		b2Fixture* mp_sensorBottom;
		b2Fixture* mp_sensorTop;
	};
}