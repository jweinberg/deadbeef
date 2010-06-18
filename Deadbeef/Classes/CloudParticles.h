/*
 *  CloudParticles.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#import "ParticleSpawnDefinition.h"
#import "Camera.h"

namespace Deadbeef
{
	class CloudParticles : public ParticleSpawnDefinition
	{
	public:
		CloudParticles(CameraRef tcamera);
		virtual ~CloudParticles();
		
		virtual void Integrate(float elapsedTime, Particle &particle);
		virtual Particle * SpawnParticle(const Vect2& origin);
		virtual void RemoveParticle(Particle * p);
		
		CameraRef mr_camera;
	};
}