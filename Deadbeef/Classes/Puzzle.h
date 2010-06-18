/*
 *  Puzzle.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/16/10.
 *  Copyright 2010  . All rights reserved.
 *
 */
#pragma once

#include "GameObject.h"
#include "Box2D.h"

namespace Deadbeef
{
	class Puzzle : public GameObject 
	{
	public:
		Puzzle(b2World* world);
		virtual ~Puzzle();
		virtual bool IsComplete(){return m_isComplete;};
		virtual void SetComplete();
		
		virtual void Update(float elapsedTime);
		virtual void Render(float elapsedTime, const Matrix& projection);
		
	protected:
		
		bool m_isComplete;
		float m_totalTime;
		
		b2World* mp_physicsWorld;
		
	};
}