/*
 *  Touch.cpp
 *  Deadbeef
 *
 *  Created by Josh Weinberg on 4/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Touch.h"

namespace Deadbeef
{
    Touch::Touch(const float tx, const float ty) : prevX(tx), prevY(ty), x(tx), y(ty)
    {
        state = kTouchBegan;
    }

    Touch::~Touch()
    {
    }
    
    void Touch::UpdateTouch(const float newX, const float newY, kTouchState newState)
    {
        prevX = x;
        prevY = y;
        
        x = newX;
        y = newY;
		
        //printf("Touch: %f,%f : %f,%f\n", prevX, prevY, x, y);
		state = newState;
    }
}