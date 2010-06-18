/*
 *  ParallaxSprite.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 4/22/10.
 *  Copyright 2010  . All rights reserved.
 *
 */
#include "ParallaxSprite.h"
#include "ShaderManager.h"
#include <iostream>
#include "AtlasManager.h"


namespace Deadbeef
{
	unsigned int ParallaxSprite::vertexBufferHandle = 0;
	unsigned int ParallaxSprite::shaderProgramHandle = 0;
	unsigned int ParallaxSprite::posAttributeLoc = 0;
	unsigned int ParallaxSprite::uvAttributeLoc = 0;
	unsigned int ParallaxSprite::wvpUniformLoc = 0;
	unsigned int ParallaxSprite::translateUVUniformLoc = 0;
	unsigned int ParallaxSprite::timeUniformLoc = 0;
	unsigned int ParallaxSprite::tex0UniformLoc = 0;
	unsigned int ParallaxSprite::tex1UniformLoc = 0;
	unsigned int ParallaxSprite::tex2UniformLoc = 0;

	ParallaxSprite::ParallaxSprite(   const std::string &spriteName0, 
								   const std::string &spriteName1,
								   const std::string &spriteName2): time(0.0f), m_uvBufferHandle(0)
	{
		shader = ShaderManager::GetInstance().ShaderNamed("Parallax");
		AtlasManager::GetInstance().TextureAndUVsNamed(spriteName0, texture[0], uvs);
		AtlasManager::GetInstance().TextureAndUVsNamed(spriteName1, texture[1], uvs);
		AtlasManager::GetInstance().TextureAndUVsNamed(spriteName2, texture[2], uvs);
	}

	ParallaxSprite::~ParallaxSprite()
	{
		glDeleteBuffers(1, &m_uvBufferHandle);
	}

	void ParallaxSprite::SetOffset(float xTrans,float yTrans)
	{
		this->xTrans = xTrans;
		this->yTrans = yTrans;
	}

	void ParallaxSprite::Update(float elapsedTime)
	{
		
	}

	void ParallaxSprite::AddChild(GameObject& child)
	{
		
		//This object is sterile...No children allowed
	}

	void ParallaxSprite::RemoveChild(GameObject& child)
	{
		
		//This object is sterile...No children allowed
	}

	void ParallaxSprite::Render(float elapsedTime, const Matrix& parentTransform) const
	{
		if(vertexBufferHandle == 0)
		{
			
			Vertex2f verts[4];

			verts[0].x = 0;
			verts[0].y = 24;
			
			verts[1].x = 0;
			verts[1].y = 0;
			
			verts[2].x = 32;
			verts[2].y = 24;
			
			verts[3].x = 32;
			verts[3].y = 0;

			
			glGenBuffers(1, &vertexBufferHandle);
			
			//Make sure OpenGL did its job
			if(vertexBufferHandle != 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
				glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2f) , verts, GL_STATIC_DRAW);
			}
		}
		
		//This shoudl be called once per object since ubBufferHandle is an instance variable
		if(m_uvBufferHandle == 0)
		{
			UV2f uvArray[4];
			uvArray[0].u = uvs.origin.x;
			uvArray[0].v = uvs.origin.y + uvs.size.height;
			
			uvArray[1].u = uvs.origin.x;
			uvArray[1].v = uvs.origin.y;
			
			uvArray[2].u = uvs.origin.x + uvs.size.width;
			uvArray[2].v = uvs.origin.y + uvs.size.height;            
			
			uvArray[3].u = uvs.origin.x + uvs.size.width;
			uvArray[3].v = uvs.origin.y;
			
			glGenBuffers(1, (GLuint*)&m_uvBufferHandle);
			if(m_uvBufferHandle != 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferHandle);
				glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(UV2f) , uvArray, GL_STATIC_DRAW);
			}
			else
			{
				//OpenGL Failed to create a uvBufferHandle
			}
		}
		
		// Update attribute values
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glEnableVertexAttribArray(posAttributeLoc);
		glVertexAttribPointer(posAttributeLoc, 2, GL_FLOAT, 0, sizeof(Vertex2f), 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferHandle);
		glEnableVertexAttribArray(uvAttributeLoc);
		glVertexAttribPointer(uvAttributeLoc, 2, GL_FLOAT, 0, sizeof(UV2f),  0);
		
		//Bind Textures
		glActiveTexture(GL_TEXTURE0 );
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]->getName()); 
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]->getName());
		
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[2]->getName());
		
		glUseProgram(shader->program);
		
		//If the shader has not been initalized yet, initialize it
		if(shaderProgramHandle == 0)
		{
			shaderProgramHandle =  shader->program;
			
			//Setup attircute handles
			posAttributeLoc = glGetAttribLocation(shaderProgramHandle, "position");
			uvAttributeLoc = glGetAttribLocation(shaderProgramHandle, "coords");
			wvpUniformLoc = glGetUniformLocation(shaderProgramHandle, "wvp");
			//Setup uniform handles
			translateUVUniformLoc = glGetUniformLocation(shaderProgramHandle, "translateUV");
			tex0UniformLoc = glGetUniformLocation(shaderProgramHandle, "tex0");
			tex1UniformLoc = glGetUniformLocation(shaderProgramHandle, "tex1");
			tex2UniformLoc = glGetUniformLocation(shaderProgramHandle, "tex2");
		}
		
		//Update Uniforms      
		glUniform2f(translateUVUniformLoc, xTrans, yTrans);
		glUniform1i(tex0UniformLoc, 0);
		glUniform1i(tex1UniformLoc, 1);
		glUniform1i(tex2UniformLoc, 2);
		glUniformMatrix4fv(wvpUniformLoc, 1, false, parentTransform.m);
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
		glDisable(GL_BLEND);
		
		glUseProgram(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glDisableVertexAttribArray(posAttributeLoc);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferHandle);
		glDisableVertexAttribArray(uvAttributeLoc);
		
		glActiveTexture(GL_TEXTURE0 );
		glBindTexture(GL_TEXTURE_2D, 0); 
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0); 
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0); 
		glDisable(GL_TEXTURE_2D);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

