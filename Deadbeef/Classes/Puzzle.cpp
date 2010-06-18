/*
 *  Puzzle.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/16/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "Puzzle.h"
#include "SoundManager.h"


namespace Deadbeef 
{
	Puzzle::Puzzle(b2World* world):
	mp_physicsWorld(world), m_isComplete(false),  m_totalTime(0)
	{
		
	}
	
	Puzzle::~Puzzle()
	{
		
	}
	
	void Puzzle::SetComplete()
	{
		m_isComplete = true;
		SoundManager::GetInstance().PlaySound("Success01", 1.0, false);
	}
	
	void Puzzle::Update(float elapsedTime)
	{
		if(m_isComplete == false)
		{
			m_totalTime += elapsedTime;
		}
		
		GameObject::Update(elapsedTime);
	}
	
	void Puzzle::Render(float elapsedTime, const Matrix& projection)
	{
		GameObject::Render(elapsedTime, projection);
	}
}