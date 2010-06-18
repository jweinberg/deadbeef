/*
 *  Sprite.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sprite.h"
#include "ShaderManager.h"
#include <iostream>
#include "AtlasManager.h"
#include "TouchManager.h"
#include <math.h>

namespace Deadbeef
{
    
	Sprite::Sprite(const std::string &spriteName) : 
		GameObject(), alpha(1.0f), position(0.0f, 0.0f), scale(1.0f, 1.0f), rotation(0.0f), currentFrame(0), frameRate(12), timeSinceFrame(0)
	{
		SetImage(spriteName);

		glGenBuffers(1, (GLuint*)&vertexBufferHandle);
		
		shader = ShaderManager::GetInstance().ShaderNamed("Shader");
			
		shaderProgramHandle =  shader->program;
		//Setup attircute handles
		posAttributeLoc = glGetAttribLocation(shaderProgramHandle, "position");
		uvAttributeLoc = glGetAttribLocation(shaderProgramHandle, "coords");
		
		//Setup uniform handles
		wvpUniformLoc = glGetUniformLocation(shaderProgramHandle, "wvp");
		alphaUniformLoc = glGetUniformLocation(shaderProgramHandle, "alpha");
		texUniformLoc = glGetUniformLocation(shaderProgramHandle, "texture");
	}

	Sprite::~Sprite()
	{
        glDeleteBuffers(1, (const GLuint*)&uvBufferHandle);
	}
	
	void Sprite::SetImage(const std::string &spriteName)
	{
		
		animationFrames = AtlasManager::GetInstance().AnimationFrames(spriteName);
		
		AtlasManager::GetInstance().TextureAndUVsNamed(animationFrames[0], m_texture, m_uvs);
		size.x = m_texture->getWidth() * CGRectGetWidth(m_uvs) / 32.0f ;
		size.y = m_texture->getHeight() * CGRectGetHeight(m_uvs) / 32.0f; 
		
		Vect2 v0 = Vect2(-size.x/2.0f, -size.y/2.0f);
		Vect2 v1 = Vect2(size.x/2.0f, -size.y/2.0f);
		Vect2 v2 = Vect2(-size.x/2.0f, size.y/2.0f);
		Vect2 v3 = Vect2(size.x/2.0f, size.y/2.0f);
		
		m_vertData[0].x = v0.x;
		m_vertData[0].y = v0.y;
		m_vertData[1].x = v1.x;
		m_vertData[1].y = v1.y;
		m_vertData[2].x = v2.x;
		m_vertData[2].y = v2.y;
		m_vertData[3].x = v3.x;
		m_vertData[3].y = v3.y;
		
		UV2f u0 = UV2f(m_uvs.origin.x,m_uvs.origin.y);
		UV2f u1 = UV2f(m_uvs.origin.x + m_uvs.size.width,m_uvs.origin.y);
		UV2f u2 = UV2f(m_uvs.origin.x,m_uvs.origin.y + m_uvs.size.height);
		UV2f u3 = UV2f(m_uvs.origin.x + m_uvs.size.width,m_uvs.origin.y + m_uvs.size.height);
		
		m_vertData[0].u = u0.u;
		m_vertData[0].v = u0.v;
		m_vertData[1].u = u1.u;
		m_vertData[1].v = u1.v;
		m_vertData[2].u = u2.u;
		m_vertData[2].v = u2.v;
		m_vertData[3].u = u3.u;
		m_vertData[3].v = u3.v;
		
	}
	

	void Sprite::Update(float elapsedTime)
	{
		//Increment the animation
		timeSinceFrame += elapsedTime;
		if (timeSinceFrame > 1.0f / frameRate)
		{
			int frameAdvance = (timeSinceFrame / (1.0f/frameRate));
			//loop
			currentFrame = (currentFrame + frameAdvance) % animationFrames.size();
			
			AtlasManager::GetInstance().TextureAndUVsNamed(animationFrames[currentFrame], m_texture, m_uvs);
			
			UV2f u0 = UV2f(m_uvs.origin.x,m_uvs.origin.y);
			UV2f u1 = UV2f(m_uvs.origin.x + m_uvs.size.width,m_uvs.origin.y);
			UV2f u2 = UV2f(m_uvs.origin.x,m_uvs.origin.y + m_uvs.size.height);
			UV2f u3 = UV2f(m_uvs.origin.x + m_uvs.size.width,m_uvs.origin.y + m_uvs.size.height);
			
			m_vertData[0].u = u0.u;
			m_vertData[0].v = u0.v;
			m_vertData[1].u = u1.u;
			m_vertData[1].v = u1.v;
			m_vertData[2].u = u2.u;
			m_vertData[2].v = u2.v;
			m_vertData[3].u = u3.u;
			m_vertData[3].v = u3.v;

			//	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
			//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VUVf), m_vertData, GL_DYNAMIC_DRAW);
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			timeSinceFrame -= (1.0f/frameRate) * frameAdvance;
		}
        GameObject::Update(elapsedTime);
	}

	Matrix Sprite::WorldMatrix() const
	{
		Matrix translationMat = Matrix::MakeTranslation(position.x, position.y, 0.0f);
		Matrix scaleMat = Matrix::MakeScale(scale.x, scale.y, 1.0f);
		Matrix rotMat = Matrix::MakeRotationZ(rotation);
		return scaleMat * rotMat * translationMat;
	}
	
	void Sprite::Render(float elapsedTime, const Matrix& parentTransform) const
	{
		unsigned int glError = 0;
		Matrix translationMat = Matrix::MakeTranslation(position.x, position.y, 0.0f);
		Matrix scaleMat = Matrix::MakeScale(scale.x, scale.y, 1.0f);
		Matrix rotMat = Matrix::MakeRotationZ(rotation);
		Matrix world = scaleMat * rotMat * translationMat * parentTransform;
		

		glUseProgram(shaderProgramHandle);
		glError = glGetError();
		
		//Enable attirbute slots
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VUVf), m_vertData, GL_DYNAMIC_DRAW);
		
		glEnableVertexAttribArray(posAttributeLoc);
		glVertexAttribPointer(posAttributeLoc, 2, GL_FLOAT, false, sizeof(VUVf), (GLvoid*)offsetof(VUVf, x));
		
		glEnableVertexAttribArray(uvAttributeLoc);
		glVertexAttribPointer(uvAttributeLoc, 2, GL_FLOAT, false, sizeof(VUVf), (GLvoid*)offsetof(VUVf, u));
		
		//Set texture 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->getName());
		
		//Set Uniform shader
		glUniformMatrix4fv(wvpUniformLoc, 1, false, world.m);
		glUniform1i(texUniformLoc, 0);
		glUniform1f(alphaUniformLoc, alpha);
		glError = glGetError();
		// Validate program before drawing. This is a good check, but only really necessary in a debug build.
		// DEBUG macro must be defined in your debug configurations if that's not already the case.
#if defined(DEBUG)
		if (!shader->Validate())
		{
			std::cerr << "Couldn't validate" << std::endl;
			return;
		}
#endif
		// Draw
		
		glEnable(GL_BLEND);
#if TARGET_IPHONE_SIMULATOR
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );   
#else
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA );   
#endif
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//debug_log("Sprite Rendered Using: VertexBuffer: %i UVBuffer: %i \n", vertexBufferHandle, uvBufferHandle);
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(posAttributeLoc);
		glDisableVertexAttribArray(uvAttributeLoc);		
		//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VUVf), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glActiveTexture(GL_TEXTURE0 );
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glError = glGetError();
		//Calls all childrens Render method
		GameObject::Render(elapsedTime, world);
	}
}
