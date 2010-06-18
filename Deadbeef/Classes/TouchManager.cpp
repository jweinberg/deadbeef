/*
 *  TouchManager.mm
 *  Deadbeef
 *
 *  Created by Josh Weinberg on 4/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TouchManager.h"
#include <iostream>
#include <math.h>
#import "MathShortcuts.h"

#define TOUCH_EPSILON 10.0f

namespace Deadbeef
{
	Swipe::Swipe() : direction(0.0f), distance(0.0f), valid(false)
	{
	}
	
	Swipe::Swipe(const float dir, const float dist) : direction(dir), distance(dist), valid(true)
	{
	}
	
	Swipe::~Swipe()
	{
	}
	
	Rotation::Rotation() : rotation(0.0f), velocity(0.0f), x(0.0f), y(0.0f), valid(false)
	{
	}
	
	Rotation::Rotation(const float tr, const float tv, const float tx, const float ty) :
		rotation(tr), velocity(tv), x(tx), y(ty), valid(true)
	{
		
	}
	
	Rotation::~Rotation()
	{
	}
	
	Pinch::Pinch() : scale(0.0f), velocity(0.0f), x(0.0f), y(0.0f), valid(false)
	{
	}
	
	Pinch::Pinch(const float tr, const float tv, const float tx, const float ty) :
	scale(tr), velocity(tv), x(tx), y(ty), valid(true)
	{
		printf("pinch with: %f\n", tr);
	}
	
	Pinch::~Pinch()
	{
	}
	
    TouchManager::TouchManager()
    {
    }


    TouchManager::~TouchManager()
    {
    }
    
	TouchList::iterator TouchManager::FindTouch(const Vect2 &point)
	{
		int closest = 0;
		float distance = 100000.0f;
		for (int i = 0; i < touches.size(); ++i)
		{
			TouchRef ref = touches[i];
			float dist = point.distance(Vect2(ref->x, ref->y));
			if (dist < distance)
			{
				distance = dist;
				closest = i;
			}
		}
		
		
		return touches.begin() + closest;
	}
	
    void TouchManager::UpdateState(const kTouchState state,const float prevX, const float prevY, const float x, const float y)
    {
        switch (state) {
            case kTouchBegan:
            {
                //Insert a new touch
                touches.push_back(Deadbeef::TouchRef(new Touch(x,y)));
            }
                break;
            case kTouchMoved:
            case kTouchEnded:
            case kTouchCancelled:
            {
                //Find the touch thats current X/Y is the same as the prev X/Y
                TouchList::iterator it = FindTouch(Vect2(prevX, prevY));
                if (it != touches.end())
                    (*it)->UpdateTouch(x,y,state);
            }
                break;
        }
        /*
        TouchList::iterator it = touches.begin();
        std::cerr << "----------------------------" << std::endl;
        for (; it < touches.end(); ++it)
        {
        
            std::cerr << "Touch at:(" << (*it)->x << ", " << (*it)->y << ")" << std::endl;
        }
        std::cerr << "----------------------------" << std::endl;
       */
    }
	
	void TouchManager::ResetGestures()
	{
		rotation = Rotation();
		pinch = Pinch();
		swipe = Swipe();
		
		TouchList::iterator it = touches.begin();
		for(; it != touches.end(); ++it)
		{
			if ((*it)->state == kTouchBegan)
				(*it)->state = kTouchHold;
            else if ((*it)->state == kTouchEnded ||
                     (*it)->state == kTouchCancelled)
            {
                it = touches.erase(it);
                it--;
            }
		}
	}

	void TouchManager::UpdatePinch(const float scale, const float velocity, const float x, const float y)
	{
		pinch = Pinch(scale, velocity, x, y);
	}
	
	void TouchManager::UpdateRotation(const float rot, const float velocity, const float x, const float y)
	{
		rotation = Rotation(rot, velocity, x, y);
	}
	
	void TouchManager::UpdateSwipe(const float distance, const float direction)
	{
		swipe = Swipe(distance, direction);
	}
}
