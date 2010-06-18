/*
 *  WindmillPuzzle.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/24/10.
 *  Copyright 2010  . All rights reserved.
 *
 */
#pragma once

#include "Puzzle.h"
#include "WindmillBase.h"
#include "WindmillLeftFoot.h"
#include "WindmillRightFoot.h"
#include "WindmillKnees.h"
#include "WindmillBody.h"
#include "WindmillHead.h"

namespace Deadbeef
{
	class WindmillPuzzle : public Puzzle 
	{

		
	public:
		WindmillPuzzle(b2World* world);
		virtual ~WindmillPuzzle();
	
		bool IsLeftFootLocked(){return m_leftFootLocked;}
		bool IsRightFootLocked(){return m_rightFootLocked;}
		
		void SetLeftFootLocked(){m_leftFootLocked = true;}
		void SetRightFootLocked(){m_rightFootLocked = true;}
		
	protected:
		WindmillBase* puzzleBase;
		WindmillLeftFoot* leftFoot;
		WindmillRightFoot* rightFoot;
		WindmillKnees* knees;
		WindmillBody* body;
		WindmillHead* head;
		
		bool m_leftFootLocked, m_rightFootLocked;		
	};
}
