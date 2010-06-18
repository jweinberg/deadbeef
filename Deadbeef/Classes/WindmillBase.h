/*
 *  WindmillBase.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/20/10.
 *  Copyright 2010  Andy Cook. All rights reserved.
 *
 */
#pragma once

#include "PuzzlePiece.h"	

namespace Deadbeef 
{

	class WindmillBase : public PuzzlePiece 
	{
	public:
		WindmillBase(b2World& world, Puzzle* parent);
		virtual ~WindmillBase();	
		
		b2Fixture* mp_sensorLeft;
		b2Fixture* mp_sensorRight;
	};
	

}

