uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

varying highp vec2 texCoord0;
varying highp vec2 texCoord1;
varying highp vec2 texCoord2;


void main()
{    
	highp vec4 pixel = mix(texture2D(tex0, texCoord0.xy), texture2D(tex1, texCoord1.xy), texture2D(tex1, texCoord1.xy).a);
	gl_FragColor = mix(pixel, texture2D(tex2, texCoord2.xy), texture2D(tex2, texCoord2.xy).a);
}
