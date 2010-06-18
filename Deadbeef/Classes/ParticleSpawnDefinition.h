/*
 *  ParticleSpawnDefinition.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "VertexFormats.h"
#include "TextureManager.h"
#include <CoreGraphics/CoreGraphics.h>
#include <tr1/memory>

namespace Deadbeef
{
	class ParticleSpawnDefinition
	{
	public:
		ParticleSpawnDefinition();
		virtual ~ParticleSpawnDefinition();
		
		virtual void Integrate(float elapsedTime, Particle &particle) = 0;
		virtual Particle * SpawnParticle(const Vect2& origin) = 0;
		virtual void RemoveParticle(Particle * p) = 0;
		float m_spawnRate;
		TextureRef mr_texture;
		CGRect m_uvs;
	};
	
    typedef std::tr1::shared_ptr<ParticleSpawnDefinition> SpawnDefinitionRef;
}