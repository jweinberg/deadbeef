//
//  Shader.fsh
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

uniform sampler2D texture;
varying lowp float alpha;
varying highp vec2 texcoord;

void main()
{
	lowp vec4 c = texture2D(texture, texcoord);
	gl_FragColor = c * alpha;
}
