/*
 *  Sprite.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Quad.h"
#include "ShaderManager.h"
#include <iostream>
#include "AtlasManager.h"
#include "TouchManager.h"
#include <math.h>

namespace Deadbeef
{
    unsigned int Quad::vertexBufferHandle = 0;

    unsigned int Quad::shaderProgramHandle = 0;
    unsigned int Quad::posAttributeLoc = 0;
    unsigned int Quad::uvAttributeLoc = 0;
    unsigned int Quad::texUniformLoc = 0;
    
	Quad::Quad(const TextureRef& ref) : texture(ref)
	{
		
		shader = ShaderManager::GetInstance().ShaderNamed("Screen");
		uvs = CGRectMake(0.0f, 0.0f, .75f, 1.0f);
        uvBufferHandle = 0;    
	}

	Quad::~Quad()
	{
        glDeleteBuffers(1, &uvBufferHandle);
	}

	void Quad::Render() const
	{
        unsigned int glError = 0;

        //This should be false once per class since vertexBufferHandle is static
        if(vertexBufferHandle == 0)
        {            
            //Temporary vertex values to be sent to GPU
            Vertex2f verts[4];
			verts[0].x = -1;
            verts[0].y = -1;
            verts[1].x = 1;
            verts[1].y = -1;
            verts[2].x = -1;
            verts[2].y = 1;
            verts[3].x = 1;
            verts[3].y = 1;
			
            glGenBuffers(1, &vertexBufferHandle);

            //Make sure OpenGL did its job
            if(vertexBufferHandle != 0)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
                glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2f) , verts, GL_STATIC_DRAW);
            }
            else 
            {
                //Opengl Failed to create a vertexBufferHandle
            }

        }
        
        //This shoudl be called once per object since ubBufferHandle it is an instance variable
        if(uvBufferHandle == 0)
        {
            UV2f uvArray[4];
            uvArray[0].u = uvs.origin.x;
            uvArray[0].v = uvs.origin.y;
            uvArray[1].u = uvs.origin.x + uvs.size.width;
            uvArray[1].v = uvs.origin.y;            
            uvArray[2].u = uvs.origin.x;
            uvArray[2].v = uvs.origin.y + uvs.size.height;
            uvArray[3].u = uvs.origin.x + uvs.size.width ;
            uvArray[3].v = uvs.origin.y + uvs.size.height;
            
            glGenBuffers(1, (GLuint*)&uvBufferHandle);
            if(uvBufferHandle != 0)
            {
                glBindBuffer(GL_ARRAY_BUFFER, uvBufferHandle);
                glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(UV2f) , uvArray, GL_STATIC_DRAW);
            }
            else
            {
                //OpenGL Failed to create a uvBufferHandle
            }
        }
        
        glUseProgram(shader->program);
        glError = glGetError();
        //If the shader has not been initalized yet, initialize it
        if(shaderProgramHandle == 0)
        {
            shaderProgramHandle =  shader->program;
            
            //Setup attircute handles
            posAttributeLoc = glGetAttribLocation(shaderProgramHandle, "position");
            uvAttributeLoc = glGetAttribLocation(shaderProgramHandle, "coords");
            
            texUniformLoc = glGetUniformLocation(shaderProgramHandle, "texture");
        }
		
        //Enable attirbute slots
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferHandle);
        glEnableVertexAttribArray(uvAttributeLoc);
        glVertexAttribPointer(uvAttributeLoc, 2, GL_FLOAT, false, sizeof(UV2f), 0);
    
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
        glEnableVertexAttribArray(posAttributeLoc);
        glVertexAttribPointer(posAttributeLoc, 2, GL_FLOAT, false, sizeof(Vertex2f), 0);

        //Set texture 
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->getName());
		
        //Set Uniform shader
		glUniform1i(texUniformLoc, 0);
	
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
        
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
        glDisableVertexAttribArray(posAttributeLoc);
		
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferHandle);
        glDisableVertexAttribArray(uvAttributeLoc);
		glUseProgram(0);        
	}
}
