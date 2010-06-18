/*
 *  WindmillPuzzle.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/24/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "WindmillPuzzle.h"

namespace Deadbeef
{
	WindmillPuzzle::WindmillPuzzle(b2World* world): Puzzle(world), m_leftFootLocked(false), m_rightFootLocked(false)
	{
        this->depth = -20;
		puzzleBase = new WindmillBase(*mp_physicsWorld, this);
		leftFoot = new WindmillLeftFoot(*mp_physicsWorld, Vect2(0, 20), this);
		rightFoot = new WindmillRightFoot(*mp_physicsWorld, Vect2(6, 20), this);
		knees = new WindmillKnees(*mp_physicsWorld, Vect2(12, 20), this);
		body = new WindmillBody(*mp_physicsWorld, Vect2(18, 20), this);
		head = new WindmillHead(*mp_physicsWorld, Vect2(24, 20), this);
		
		AddChild(*head);
		AddChild(*puzzleBase);
		AddChild(*leftFoot);
		AddChild(*rightFoot);
		AddChild(*knees);
		AddChild(*body);
		AddChild(*head);
	}
	
	WindmillPuzzle::~WindmillPuzzle()
	{
		
	}
}