//
//  Shader.fsh
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

uniform lowp vec3 color;

void main()
{
	gl_FragColor = vec4(color, .6);
}
