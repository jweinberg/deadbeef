/*
 *  ShaderManager.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ShaderManager.h"

#include "TextureManager.h"
#include "Texture.h"
#include <tr1/memory>
#include "CocoaWrapper.h"
#include <iostream>

namespace Deadbeef
{
	ShaderManager::ShaderManager()
	{
	}
	
	ShaderManager::~ShaderManager()
	{
	}
	
	
	ShaderRef ShaderManager::ShaderNamed(const std::string &name)
	{
		ShaderMap::iterator it = shaders.find(name);
		if (it == shaders.end())
		{
			//texture not loaded
			std::cerr << "Loading new shader" << std::endl;
			ShaderRef ref = ShaderRef(new Shader(name));
			shaders.insert(std::pair<std::string, ShaderRef>(name,ref));
			return ref;
		}
		return it->second;
	}
	
}