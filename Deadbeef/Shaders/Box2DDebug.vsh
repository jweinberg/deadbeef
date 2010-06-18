//
//  Shader.vsh
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//
uniform mat4 wvp;
attribute vec4 position;
 

void main()
{
    vec4 transformed;
    transformed = position;
    transformed = wvp * transformed;
	gl_Position =  transformed;
}