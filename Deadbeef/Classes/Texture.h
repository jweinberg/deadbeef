/*
 *  Texture.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include <string>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include <tr1/memory>


namespace Deadbeef
{
	class Texture
	{
	public:
		Texture();
		Texture(const std::string &path);
		Texture(GLuint name, float s, float t, float textureWidth, float textureHeight, float fullWidth, float fullHeight);
		virtual ~Texture();
	private:
		float s;
		float t;
		
		float textureWidth;
		float textureHeight;
		
		float fullWidth;
		float fullHeight;
		
		GLuint name;
		
	public:
		
		GLuint getName() { return name; }
		float getS() { return s; }
		float getT() { return t; }
		float getWidth() { return textureWidth; }
		float getHeight() { return textureHeight; }

		
	};
	
	typedef std::tr1::shared_ptr<Texture> TextureRef;
    typedef std::tr1::weak_ptr<Texture> TextureWeakRef;
}
