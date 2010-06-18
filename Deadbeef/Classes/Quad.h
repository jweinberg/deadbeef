/*
 *  Sprite.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "GameObject.h"
#include "Texture.h"
#include "Vect2.h"
#include "VertexFormats.h"
#include "Shader.h"
#import "Matrix.h"
#import <CoreGraphics/CoreGraphics.h>
#include "math.h"

namespace Deadbeef
{
		
	class Quad
	{
	public:        
        //Shader Data
        static unsigned int vertexBufferHandle;
        unsigned int uvBufferHandle;
        static unsigned int shaderProgramHandle;
        //Attribute Handles
        static unsigned int posAttributeLoc;
        static unsigned int uvAttributeLoc;
        //Uniform Handles
        static unsigned int wvpUniformLoc;
        static unsigned int texUniformLoc;
		
        Quad(const TextureRef& ref);
		virtual ~Quad();
		virtual void Render() const;

	protected:
        static unsigned int count;
		TextureRef texture;
		CGRect uvs;
		ShaderRef shader;
	};	
	
}