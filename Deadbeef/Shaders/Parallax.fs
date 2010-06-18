uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
varying vec2 texCoord0;
varying vec2 texCoord1;
varying vec2 texCoord2;
varying vec2 texCoord3;

void main()
{
	vec4 pixel;
	pixel.rgb = texture2D(tex0, gl_TexCoord[0].xy).rgb;
	if(texture2D(tex1, gl_TexCoord[1].xy).a > 0.0)
	{
		pixel.rgb = mix(pixel.rgb, texture2D(tex1, gl_TexCoord[1].xy).rgb, 			texture2D(tex1, gl_TexCoord[1].xy).a);
	}

	if(texture2D(tex2, gl_TexCoord[2].xy).a > 0.0)
	{
		pixel.rgb = mix(pixel.rgb, texture2D(tex2, gl_TexCoord[2].xy).rgb, 					texture2D(tex2, gl_TexCoord[2].xy).a);
	}

	if(texture2D(tex3, gl_TexCoord[3].xy).a > 0.0)
	{
		pixel.rgb = mix(pixel.rgb, texture2D(tex2, gl_TexCoord[3].xy).rgb, 					texture2D(tex3, gl_TexCoord[3].xy).a);
	}
	
	gl_FragColor = pixel;

}
