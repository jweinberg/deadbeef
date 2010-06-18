/*
 *  TextureManager.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureManager.h"
#include "Texture.h"
#include <tr1/memory>
#include "CocoaWrapper.h"
#include <iostream>

namespace Deadbeef
{
	TextureManager::TextureManager()
	{
	}
	
	TextureManager::~TextureManager()
	{
	}
	
	TextureRef TextureManager::TextureNamed(const std::string &name)
	{
		TextureMap::iterator it = textures.find(name);
		if (it == textures.end())
		{
			//texture not loaded
			std::cerr << "Loading new texture" << std::endl;
			TextureRef ref = TextureRef(new Texture(CocoaWrapper::PathForResource(name, "png"), 250, 327));
			textures.insert(std::pair<std::string, TextureRef>(name,ref));
			return ref;
		}
		return it->second;
	}
}