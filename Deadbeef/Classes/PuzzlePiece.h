/*
 *  GamePiece.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/16/10.
 *  Copyright 2010  Andy Cook. All rights reserved.
 */
#include "PhysicsSprite.h"
#include "Puzzle.h"

#pragma once

namespace Deadbeef
{	
	enum PUZZLE_PIECE_TYPE 
	{
		kPuzzleBase,
		kPuzzleWindmillBase,
		kPuzzleWindmillLeftFoot,
		kPuzzleWindmillRightFoot,
		kPuzzleWindmillKnees,
		kPuzzleWindmillBody,
		kPuzzleWindmillHead
	};
	
	class PuzzlePiece : public PhysicsSprite
	{		
	public:
		PuzzlePiece(const std::string& imageName, b2World& world, const Vect2& position, Puzzle* parent);

		virtual ~PuzzlePiece();
		bool IsLinked(){return m_linked;}
		
	protected:
		bool m_linked;
		Puzzle* mp_parentPuzzle;
	};
}