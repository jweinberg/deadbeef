/*
 *  ParticleEmitter.h
 *  Deadbeef
 *
 *  Created by Andy Cook on 5/15/10.
 *  Copyright 2010  Andy Cook. All rights reserved.
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
#include "ParticleSpawnDefinition.h"
#include <list>

namespace Deadbeef
{
	class ParticleEmitter : public GameObject
	{
	public:        
        Vect2 m_origin;
        Vect2 m_size;
        Vect2 m_scale;
		
        //Shader Data
		unsigned int m_vertexBufferHandle;
        unsigned int m_uvBufferHandle;
        unsigned int m_shaderProgramHandle;
        //Attribute Handles
		int m_posAttributeLoc;
		int m_uvAttributeLoc;
		int m_alphaAttributeLoc;
        //Uniform Handles
		int m_wvpUniformLoc;
		int m_texUniformLoc;
		int m_activeParticles;
		
        ParticleEmitter(Vect2 origin, unsigned int particleCount, SpawnDefinitionRef mr_spawnDef);
		virtual ~ParticleEmitter();
		void Update(float elapsedTime);
		void Render(float elapsedTime, const Matrix& projection) const;
		void Activate(){m_active = true;}
		void Deactivate(){m_active = false;}
		
	protected:
        static unsigned int s_systemCount;
		unsigned int m_particleCount;
		bool m_active;
		bool m_visible;
		SpawnDefinitionRef mr_spawnDef;
		ShaderRef mr_shader;
//TODO:OPTIMIZE ME AWAY
		std::list<Particle*> m_particles;
		PVert * mp_vertData;
		float m_timeSinceLast;
		
	};
	typedef std::tr1::shared_ptr<ParticleEmitter> ParticleEmitterRef;
}