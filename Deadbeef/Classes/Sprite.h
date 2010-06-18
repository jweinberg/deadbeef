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
		
	class Sprite : public GameObject
	{
	public:        
        Vect2 position;
        Vect2 size;
        Vect2 trans;
        Vect2 scale;
        float rotation;
		float frameRate;
		float alpha;
        //Shader Data
		int vertexBufferHandle;
		int uvBufferHandle;
		int shaderProgramHandle;
        //Attribute Handles
		int posAttributeLoc;
		int uvAttributeLoc;
        //Uniform Handles
		int alphaUniformLoc;
		int wvpUniformLoc;
		int texUniformLoc;
		
        Sprite(const std::string& spriteName);
		virtual ~Sprite();
        virtual void Update(float elapsedTime);
		virtual void Render(float elapsedTime, const Matrix& projection) const;
		void SetImage(const std::string &spriteName);
		Matrix WorldMatrix() const;
		
		const Vect2* GetPosition(){return &position;}
	protected:
		std::vector<std::string> animationFrames;
		int currentFrame;
		float timeSinceFrame;
		TextureRef m_texture;
		CGRect m_uvs;
		ShaderRef shader;
		VUVf m_vertData[4];
	};
	
	typedef std::tr1::shared_ptr<Sprite> SpriteRef;
	
	
}