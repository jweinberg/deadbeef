/*
 *  VertexFormat.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/4/10.
 *  Copyright 2010  Andrew Cook All rights reserved.
 */

#pragma once
#include "Vect2.h"

#include "Vect2.h"
#include <stdio.h>

namespace Deadbeef
{
    struct Vertex2f 
    {
        float x;
        float y;
        Vertex2f(): x(0), y(0)
        {
            
        }
    };
    
    struct UV2f 
    {
        float u;
        float v;
        UV2f(): u(0), v(0)
        {
            
        }
		UV2f(float tu, float tv) : u(tu), v(tv)
        {
            
        }
    };
	
	struct VUVf
	{
		float x;
		float y;
		float u;
		float v;
	};
	
	struct PVert
	{
		float x;
		float y;
		float u;
		float v; 
		float a;
	};
	
	struct Particle 
    {
        float xPos;
		float yPos;
		float xVel;
		float yVel;
		float xAccel;
		float yAccel;
		float xScale;
		float yScale;
		float a;
		float life;
		float rotation;
		
		Particle(): xPos(0), yPos(0), xVel(0), yVel(0), xAccel(0), yAccel(0), xScale(1.0f), yScale(1.0f), a(1.0f), rotation(0.0f)
		{
			
		}
		
		Particle(float x, float y, float xv, float yv, float xa, float ya): 
		xPos(x), yPos(y), xVel(xv), yVel(yv), xAccel(xa), yAccel(ya)
		{
			
		}
    };
}