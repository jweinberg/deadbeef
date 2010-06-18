//
//  Shader.fsh
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

uniform sampler2D texture;
varying highp vec2 texcoord;

void main()
{
	gl_FragColor = texture2D(texture, texcoord);
}
