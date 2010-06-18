/*
 *  Shader.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Shader.h"
#include <string>
#include "CocoaWrapper.h"
#include <fstream>
#include <iostream>

namespace Deadbeef
{
	
	Shader::Shader(const std::string& name)
	{
		GLuint vertShader, fragShader;
		std::string vertShaderPathname, fragShaderPathname;
		
		// Create shader program
		program = glCreateProgram();
		
		// Create and compile vertex shader
		vertShaderPathname = CocoaWrapper::PathForResource(name, "vsh");
		if (!Shader::CompileShader(&vertShader, GL_VERTEX_SHADER, vertShaderPathname))
		{
			std::cerr << "Failed to compile vertex shader" << std::endl;
		}
		
		// Create and compile fragment shader
		fragShaderPathname =CocoaWrapper::PathForResource(name, "fsh");
		if (!Shader::CompileShader(&fragShader, GL_FRAGMENT_SHADER, fragShaderPathname))
		{
			std::cerr << "Failed to compile fragment shader" << std::endl;
		}
		
		// Attach vertex shader to program
		glAttachShader(program, vertShader);
		
		// Attach fragment shader to program
		glAttachShader(program, fragShader);
		
		// Bind attribute locations
		// this needs to be done prior to linking
		glBindAttribLocation(program, ATTRIB_VERTEX, "position");
		glBindAttribLocation(program, ATTRIB_TEX_COORD, "coords");
		
		// Link program
		if (!Shader::LinkShader(program))
		{
			std::cerr << "Failed to link program: " << program << std::endl;
			
			if (vertShader)
			{
				glDeleteShader(vertShader);
				vertShader = 0;
			}
			if (fragShader)
			{
				glDeleteShader(fragShader);
				fragShader = 0;
			}
			if (program)
			{
				glDeleteProgram(program);
				program = 0;
			}	
		}
		
		// Get uniform locations
        uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(program, "texture");
		// Release vertex and fragment shaders
		if (vertShader)
			glDeleteShader(vertShader);
		if (fragShader)
			glDeleteShader(fragShader);
	}
	
	Shader::~Shader()
	{
		glDeleteProgram(program);
	}
	
	void Shader::use() const
	{
		glUseProgram(program);
	}
	
	const bool Shader::Validate() const
	{
		GLint logLength, status;
		
		glValidateProgram(program);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			GLchar *log = (GLchar *)malloc(logLength);
			glGetProgramInfoLog(program, logLength, &logLength, log);
			std::cerr << "Program validate log: \n" << log << std::endl;
			free(log);
		}
		
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		if (status == 0)
		{
			std::cerr << "Failed to validate program: \n" << program << std::endl;
			return false;
		}
		
		return true;
	}

	
	const bool Shader::LinkShader(GLuint prog)
	{
		GLint status;
		
		glLinkProgram(prog);
		
#if defined(DEBUG)
		GLint logLength;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			GLchar *log = (GLchar *)malloc(logLength);
			glGetProgramInfoLog(prog, logLength, &logLength, log);
			std::cerr << "Program link log:" << log << std::endl;
			free(log);
		}
#endif
		
		glGetProgramiv(prog, GL_LINK_STATUS, &status);
		if (status == 0)
			return false;
		
		return true;
	}
	
	
	const bool Shader::CompileShader(GLuint *shader, GLenum type, const std::string &file)
	{    
		GLint status;

		
		std::ifstream inFile(file.c_str(), std::ios::binary);
		inFile.seekg(0, std::ios_base::end);
		long length = inFile.tellg();
		inFile.seekg(0, std::ios_base::beg);
		GLchar * const source = new GLchar[length + 1];
        
		inFile.read(source, length);
		inFile.close();
        source[length] = '\0';

		if (!source)
		{
			std::cerr << "Failed to load vertex shader" << std::endl;
			return false;
		}
		
		*shader = glCreateShader(type);
		glShaderSource(*shader, 1, (const GLchar**)&source, NULL);
		glCompileShader(*shader);
		
		delete [] source;
		
#if defined(DEBUG)
		GLint logLength;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			GLchar *log = (GLchar *)malloc(logLength);
			glGetShaderInfoLog(*shader, logLength, &logLength, log);
			std::cerr << "Shader compile log:\n" << log << std::endl;
			free(log);
		}
#endif
		
		glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
		if (status == 0)
		{
			glDeleteShader(*shader);
			return false;
		}
		
		return true;
	}
}