/*
 *  WindmillRightFoot.h
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
	class PuzzlePiece;
	
	class WindmillRightFoot : public PuzzlePiece 
	{
		static const float xLock = 17.8;
		static const float yLock = 20.3;
		
	public:
		WindmillRightFoot(b2World& world, Vect2 position, Puzzle* parent);
		virtual ~WindmillRightFoot();
		
		void Update(float elapsedTime);
	
	protected:
		b2Fixture* mp_sensorBottom;
		b2Fixture* mp_sensorTop;
	};
}