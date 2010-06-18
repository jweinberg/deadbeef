/*
 *  ParticleEmitter.cpp
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/15/10.
 *  Copyright 2010  . All rights reserved.
 *
 */

#include "ParticleEmitter.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include "ShaderManager.h"
#include "AtlasManager.h"
#include "Vect2.h"

namespace Deadbeef
{
	ParticleEmitter::ParticleEmitter(Vect2 origin, unsigned int particleCount, SpawnDefinitionRef mr_spawnDef):
	m_origin(origin), m_particleCount(particleCount), m_active(true), m_visible(true), m_timeSinceLast(0)
	{
		depth = 100;
		mp_vertData = new PVert[m_particleCount * 6];
		
		glGenBuffers(1, &m_vertexBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(PVert) * particleCount, NULL, GL_DYNAMIC_DRAW);

		mr_shader = ShaderManager::GetInstance().ShaderNamed("Particle");
		
		m_shaderProgramHandle =  mr_shader->program;
		
		m_scale.x = 1.0f;
        m_scale.y = 1.0f;
		m_size.x = mr_spawnDef->mr_texture->getWidth() * CGRectGetWidth(mr_spawnDef->m_uvs) / 64.0f;
		m_size.y = mr_spawnDef->mr_texture->getHeight() * CGRectGetHeight(mr_spawnDef->m_uvs) / 64.0f; 

		
		//Setup attircute handles
		m_posAttributeLoc = glGetAttribLocation(m_shaderProgramHandle, "position");
		m_uvAttributeLoc = glGetAttribLocation(m_shaderProgramHandle, "coords");
		m_alphaAttributeLoc = glGetAttribLocation(m_shaderProgramHandle, "alphas"); 
		//Setup uniform handles
		m_wvpUniformLoc = glGetUniformLocation(m_shaderProgramHandle, "wvp");
		m_texUniformLoc = glGetUniformLocation(m_shaderProgramHandle, "mr_texture");
	
		this->mr_spawnDef = mr_spawnDef;
		
		m_activeParticles = 0;
		unsigned int i;

		for (i = 0; i < m_particleCount; ++i) 
		{
			UV2f u0 = UV2f(mr_spawnDef->m_uvs.origin.x,mr_spawnDef->m_uvs.origin.y);
			UV2f u1 = UV2f(mr_spawnDef->m_uvs.origin.x + mr_spawnDef->m_uvs.size.width,mr_spawnDef->m_uvs.origin.y);
			UV2f u2 = UV2f(mr_spawnDef->m_uvs.origin.x,mr_spawnDef->m_uvs.origin.y + mr_spawnDef->m_uvs.size.height);
			UV2f u3 = UV2f(mr_spawnDef->m_uvs.origin.x + mr_spawnDef->m_uvs.size.width,mr_spawnDef->m_uvs.origin.y + mr_spawnDef->m_uvs.size.height);
			mp_vertData[(i * 6)].u = u0.u;
			mp_vertData[(i * 6)].v = u0.v;
			mp_vertData[(i * 6)+1].u = u1.u;
			mp_vertData[(i * 6)+1].v = u1.v;
			mp_vertData[(i * 6)+2].u = u2.u;
			mp_vertData[(i * 6)+2].v = u2.v;
			mp_vertData[(i * 6)+3].u = u3.u;
			mp_vertData[(i * 6)+3].v = u3.v;			
			mp_vertData[(i * 6)+4].u = u2.u;
			mp_vertData[(i * 6)+4].v = u2.v;
			mp_vertData[(i * 6)+5].u = u1.u;
			mp_vertData[(i * 6)+5].v = u1.v;
		}
	}
	
	ParticleEmitter::~ParticleEmitter()
	{
		delete [] mp_vertData;
		
		glDeleteBuffers(1, &m_vertexBufferHandle);
	}
	#define MIN(A,B) ((A)<(B)?(A):(B))
	
	void ParticleEmitter::Update(float elapsedTime)
	{
		int currentSize = m_particles.size();
		if(m_active)
		{
			if (currentSize < m_particleCount)
			{
				m_timeSinceLast+=elapsedTime;
				//printf("time: %f\n", m_timeSinceLast);
				if (m_timeSinceLast >= mr_spawnDef->m_spawnRate)
				{
					
					int count = MIN(m_particleCount - currentSize,m_timeSinceLast / mr_spawnDef->m_spawnRate);
					//printf("spawning: %d %f %f\n", count, m_timeSinceLast, elapsedTime);
					for (; count > 0; --count)
					{
						m_particles.push_back(mr_spawnDef->SpawnParticle(m_origin));
					}
					m_timeSinceLast = 0.0f;

				}
			}
		}
		
		//Clean dead m_particles
		m_activeParticles = 0;
		int i = 0;
		std::list<Particle*>::iterator it = m_particles.begin();
		for (;it != m_particles.end(); ++it)
		{

			Particle * p = (*it);
			mr_spawnDef->Integrate(elapsedTime, *p);
			
			if (p->life <= 0.0f)
			{
				it = m_particles.erase(it);
				it--; //Screw you double inc!
		 		mr_spawnDef->RemoveParticle(p);
			}
			else
			{
				Vect2 pSize = m_size * Vect2(p->xScale, p->yScale);
				Vect4 v = Vect4(p->xPos, p->yPos, 0, 1);
				//v is the center point
				
				Matrix rotationMat = Matrix::MakeRotationZ(p->rotation);
				
				
				Vect4 v0 = v - rotationMat * Vect4(pSize.x/2.0f, pSize.y/2.0f, 0, 1);
				Vect4 v1 = v - rotationMat * Vect4(-pSize.x/2.0f, pSize.y/2.0f, 0, 1);
				Vect4 v2 = v - rotationMat * Vect4(pSize.x/2.0f, -pSize.y/2.0f, 0, 1);
				Vect4 v3 = v - rotationMat * Vect4(-pSize.x/2.0f, -pSize.y/2.0f, 0, 1);
				
				//Rotate these bitches!
				
				
				i = m_activeParticles * 6;
				mp_vertData[i].x = v0.x;
				mp_vertData[i].y = v0.y; 		
				mp_vertData[i+1].x = v1.x;
				mp_vertData[i+1].y = v1.y;			
				mp_vertData[i+2].x = v2.x;
				mp_vertData[i+2].y = v2.y;			
				mp_vertData[i+3].x = v3.x;
				mp_vertData[i+3].y = v3.y;			
				mp_vertData[i+4].x = v2.x;
				mp_vertData[i+4].y = v2.y;			
				mp_vertData[i+5].x = v1.x;
				mp_vertData[i+5].y = v1.y;		
				
				
				mp_vertData[i].a = p->a;
				mp_vertData[i+1].a = p->a;
				mp_vertData[i+2].a = p->a;
				mp_vertData[i+3].a = p->a;
				mp_vertData[i+4].a = p->a;
				mp_vertData[i+5].a = p->a;
				
				m_activeParticles++;
			}
		}
	}
	
	void ParticleEmitter::Render(float elapsedTime, const Matrix& projection) const
	{
		if (m_visible) 
		{
			unsigned int glError = 0;
			//Matrix translationMat = Matrix::MakeTranslation(m_origin.x, m_origin.y, 0.0f);
			//Matrix scaleMat = Matrix::MakeScale(size.x * scale.x, size.y * m_vertexBufferHandle.y, 1.0f);
			
			Matrix world = projection;
	
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
			glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(PVert) * m_activeParticles, mp_vertData, GL_DYNAMIC_DRAW);
			
			glUseProgram(mr_shader->program);
			glError = glGetError();
			
			//Enable attirbute slots
			
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
			glEnableVertexAttribArray(m_posAttributeLoc);
			glVertexAttribPointer(m_posAttributeLoc, 2, GL_FLOAT, false, sizeof(PVert), (GLvoid*)offsetof(PVert,x));
			
			glEnableVertexAttribArray(m_uvAttributeLoc);
			glVertexAttribPointer(m_uvAttributeLoc, 2, GL_FLOAT, false, sizeof(PVert), (GLvoid*)offsetof(PVert,u));
			
			glEnableVertexAttribArray(m_alphaAttributeLoc);
			glVertexAttribPointer(m_alphaAttributeLoc, 1, GL_FLOAT, false, sizeof(PVert), (GLvoid*)offsetof(PVert,a));
			
			//Set mr_texture 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mr_spawnDef->mr_texture->getName());
			
			//Set Uniform shader
			glUniformMatrix4fv(m_wvpUniformLoc, 1, false, world.m);
			glUniform1i(m_texUniformLoc, 0);
			
			glError = glGetError();
			// Validate program before drawing. This is a good check, but only really necessary in a debug build.
			// DEBUG macro must be defined in your debug configurations if that's not already the case.
#if defined(DEBUG)
			if (!mr_shader->Validate())
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
			glDrawArrays(GL_TRIANGLES, 0, m_activeParticles * 6);
			//debug_log("Sprite Rendered Using: VertexBuffer: %i UVBuffer: %i \n", vertexBufferHandle, m_uvBufferHandle);
			glDisable(GL_BLEND);
			
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(PVert) * m_activeParticles * 6, NULL, GL_DYNAMIC_DRAW);
			glDisableVertexAttribArray(m_posAttributeLoc);
			glDisableVertexAttribArray(m_uvAttributeLoc);			
			glDisableVertexAttribArray(m_alphaAttributeLoc);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			glActiveTexture(GL_TEXTURE0 );
			glBindTexture(GL_TEXTURE_2D, 0);
			
			glError = glGetError();
			//Calls all childrens Render method
			GameObject::Render(elapsedTime, world);
			
		}
	}
}