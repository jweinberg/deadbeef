/*
 *  ParticleSpawnDefinition.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleSpawnDefinition.h"
#include <stdlib.h>
#include "AtlasManager.h"

namespace Deadbeef
{
	ParticleSpawnDefinition::ParticleSpawnDefinition()
	{
		m_spawnRate = .01f;
		AtlasManager::GetInstance().TextureAndUVsNamed("test:smoke_particle", mr_texture, m_uvs);
	}
	
	ParticleSpawnDefinition::~ParticleSpawnDefinition()
	{
		
	}
#define MAX(A,B) ((A)>(B)?(A):(B))
	void ParticleSpawnDefinition::Integrate(float elapsedTime, Particle& particle)
	{
		particle.xVel += elapsedTime * particle.xAccel;
		particle.yVel += elapsedTime * particle.yAccel;		
		
		particle.xAccel = MAX(0.0f, particle.xAccel - .1f);
		particle.yAccel *= .5f;
		
		particle.xPos += particle.xVel;
		particle.yPos += particle.yVel;
		
		particle.life -= elapsedTime;	
		
		particle.a = particle.life / 3.5f;
		particle.rotation += 5.0f;
	}
	
	Particle * ParticleSpawnDefinition::SpawnParticle(const Vect2& origin)
	{
		Particle * p = new Particle();
		p->xAccel = 0;// (float)(rand() % 100) / 100.0f - .5f;
		p->yAccel = -(float)(rand() % 100) / 100.0f;
		p->xPos = (float)(rand() % 100) / 100.f - .5f + origin.x;
		p->yPos = (float)(rand() % 100) / 100.f - .5f + origin.y;
		p->life = 3.5f;
		
		float scale = 1.f + (float)(rand() % 10)/10.f * 2.0f;
		p->xScale = scale;
		p->yScale = scale;
		return p;
	}
	
	void ParticleSpawnDefinition::RemoveParticle(Particle * p)
	{
		//This should queue these up, so as to not keep churning memory
		delete p;
	}
}