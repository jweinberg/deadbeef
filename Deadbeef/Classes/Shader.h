/*
 *  Shader.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <string>
#include <tr1/memory>

namespace Deadbeef
{
	// uniform index
	enum {
        UNIFORM_WVP,
		UNIFORM_TEXTURE,
		NUM_UNIFORMS
	};
	
	// attribute index
	enum {
		ATTRIB_VERTEX,
		ATTRIB_TEX_COORD,
		NUM_ATTRIBUTES
	};
	
	
	class Shader
	{
	public:
		Shader(const std::string& name);
		virtual ~Shader();
		
		const bool Validate() const;
		void use() const;
		
		GLint uniforms[NUM_UNIFORMS];
		
		const bool CompileShader(GLuint *shader, GLenum type, const std::string &file);
		const bool LinkShader(GLuint prog);

		
		GLuint program;
	};
	
	typedef std::tr1::shared_ptr<Shader> ShaderRef;
}