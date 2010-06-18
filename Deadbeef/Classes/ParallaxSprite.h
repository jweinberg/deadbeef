/*
 *  ParallaxSprite.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 4/22/10.
 *  Copyright 2010  . All rights reserved.
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
	class ParallaxSprite : public GameObject
	{
	public:
        ParallaxSprite(const std::string& spriteName0, 
					   const std::string& spriteName1, 
					   const std::string& spriteName2);
        
		virtual ~ParallaxSprite();
		virtual void Render(float elapsedTime, const Matrix& parentTransform) const;
        virtual void Update(float elapsedTime);
        
        //Override so they cannot be used
        virtual void AddChild(GameObject& child);
        virtual void RemoveChild(GameObject& child);
        
        void SetOffset(float xTrans, float yTrans);
        
    protected:
        static unsigned int vertexBufferHandle;
        static unsigned int shaderProgramHandle;
        static unsigned int posAttributeLoc;
        static unsigned int uvAttributeLoc;
        static unsigned int wvpUniformLoc;
        static unsigned int translateUVUniformLoc;
        static unsigned int timeUniformLoc;
        static unsigned int tex0UniformLoc;
        static unsigned int tex1UniformLoc;
        static unsigned int tex2UniformLoc;
        static unsigned int count;
		
		unsigned int m_uvBufferHandle;
		TextureRef texture[3];
		CGRect uvs;
        Vect4 parallaxAmount;
		ShaderRef shader;
        float time;
        float xTrans;
        float yTrans;
	};
	
	typedef std::tr1::shared_ptr<ParallaxSprite> ParallaxSpriteRef;
}
