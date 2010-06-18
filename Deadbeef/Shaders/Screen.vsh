attribute vec4 position;
attribute vec2 coords;
varying vec2 texcoord;

void main()
{
	gl_Position = position;
	texcoord = coords;
}