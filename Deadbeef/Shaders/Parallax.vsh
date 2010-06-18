//Parallax.vert
//Author: Andrew Cook
//Description: Uses

uniform vec2 translateUV;
uniform mat4 wvp;

attribute vec4 position;
attribute vec4 coords;

varying highp vec2 texCoord0;
varying highp vec2 texCoord1;
varying highp vec2 texCoord2;

void main()
{
    vec3 amount = vec3(0.2, 0.4, 1.0);
    
	texCoord0 =  coords.xy + translateUV * amount.r; 
    texCoord1 =  coords.xy + translateUV * amount.g; 
    texCoord2 =  coords.xy + translateUV * amount.b; 

	gl_Position =  wvp * position;
}
