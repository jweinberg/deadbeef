//Parallax.vert
//Author: Andrew Cook
//Description: Uses


//Camera Matrix
uniform mat4 camera;

//Stores 4 levels of parallax designated by (r,g,b,a)
uniform vec4 amount;

attribute vec4 position;
attribute vec4 color;
attribute vec4 coords;

varying vec2 texCoord0;
varying vec2 texCoord1;
varying vec2 texCoord2;
varying vec2 texCoord3;

void main()
{
	mat4 transform = camera;
	transform[3][0] *= amount.r; // Adjust X translate
	transform[3][1] *= amount.r; // Adjust Y translate
	transform[3][2] *= amount.r; // Adjust Z translate
	texCoord0 = (transform * coords).xy;
	//gl_Position = position;
	gl_TexCoord[0].xy =(transform * gl_MultiTexCoord0).xy; 

	transform = camera;
	transform[3][0] *= amount.g; // Adjust X translate
	transform[3][1] *= amount.g; // Adjust Y translate
	gl_TexCoord[1].xy =(transform * gl_MultiTexCoord0).xy; 

	transform = camera;
	transform[3][0] *= amount.b; // Adjust X translate
	transform[3][1] *= amount.b; // Adjust Y translate
	gl_TexCoord[2].xy =(transform * gl_MultiTexCoord0).xy; 

	transform = camera;
	transform[3][0] *= amount.a; // Adjust X translate
	transform[3][1] *= amount.a; // Adjust Y translate
	gl_TexCoord[3].xy =(transform * gl_MultiTexCoord0).xy; 

	gl_FrontColor = color;
	gl_Position = ftransform();
}
