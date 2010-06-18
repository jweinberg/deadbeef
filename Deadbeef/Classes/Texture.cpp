/*
 *  Texture.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "Texture.h"
#import <CoreGraphics/CoreGraphics.h>

namespace Deadbeef
{
	Texture::Texture()
	{
	}

	Texture::Texture(GLuint _name, float _s, float _t, float _textureWidth, float _textureHeight, float _fullWidth, float _fullHeight) : name(_name), s(_s), t(_t), textureWidth(_textureWidth), textureHeight(_textureHeight), fullWidth(_fullWidth), fullHeight(_fullHeight)
	{
	}
	
	Texture::Texture(const std::string &path)
	{
		const CGDataProviderRef provider = CGDataProviderCreateWithFilename(path.c_str());
		const CGImageRef aImage = CGImageCreateWithPNGDataProvider(provider, NULL, 0, kCGRenderingIntentDefault);
		const CFDataRef  imageData  = CGDataProviderCopyData( CGImageGetDataProvider( aImage ) ); 	
		
		textureWidth = fullWidth = CGImageGetWidth( aImage );
		textureHeight = fullHeight = CGImageGetHeight( aImage );
			

		const void* data = CFDataGetBytePtr( imageData );
		
		glGenTextures(1, &name);
		
		glBindTexture(GL_TEXTURE_2D, name);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		//Added by Andy to hack the background for "Ground Up"
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); 
		
	#if TARGET_IPHONE_SIMULATOR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fullWidth, fullHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);		
	#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fullWidth, fullHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	#endif
		
		s = (float)textureWidth / (float)fullWidth;
		t = (float)textureHeight / (float)fullHeight;
		
		
		CFRelease(imageData);
		CGImageRelease(aImage);
		CGDataProviderRelease(provider);	
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &name);
	}

}
