/*
 *  CloudParticles.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CloudParticles.h"
#include <stdlib.h>
#include "AtlasManager.h"

namespace Deadbeef
{
	CloudParticles::CloudParticles(CameraRef tcamera) : mr_camera(tcamera)
	{
		m_spawnRate = 2.0f;
		AtlasManager::GetInstance().TextureAndUVsNamed("Windmill:CLOUD5", mr_texture, m_uvs);
	}
	
	CloudParticles::~CloudParticles()
	{
		
	}
#define MAX(A,B) ((A)>(B)?(A):(B))
	void CloudParticles::Integrate(float elapsedTime, Particle& particle)
	{
		particle.xVel += elapsedTime * particle.xAccel;
		particle.yVel += elapsedTime * particle.yAccel;		
		
		particle.xAccel = MAX(0.0f, particle.xAccel - .1f);
		particle.yAccel *= .5f;
		
		particle.xPos += particle.xVel;
		particle.yPos += particle.yVel;
	
		
		if (particle.xPos - 25 > mr_camera->GetPosition().x + 36.0f)
			particle.life = 0.0f;
		
		//particle.a = particle.life / 10.0f;
	}
	
	Particle * CloudParticles::SpawnParticle(const Vect2& origin)
	{
		Particle * p = new Particle();
		p->xAccel = (float)(rand() % 100) / 100.0f * .5f + .5f; // .5(rand) + .5 random in the range of .5-1.0
		p->yAccel = 0;
		p->xPos = origin.x - 25.0;
		p->yPos = (float)(rand() % 15) + origin.y - 4;
		p->life = 30.0f;
		
		float scale = 2.f + (float)(rand() % 10)/10.f * 2.0f;
		p->xScale = scale;
		p->yScale = scale;
		return p;
	}
	
	void CloudParticles::RemoveParticle(Particle * p)
	{
		//This should queue these up, so as to not keep churning memory
		delete p;
	}
}