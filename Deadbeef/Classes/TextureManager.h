/*
 *  TextureManager.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Singleton.h"
#include <string>
#include <map>
#include "Texture.h"

namespace Deadbeef
{
	class Texture;
	
	typedef std::map<std::string, TextureRef> TextureMap;
	
	class TextureManager : public Singleton<TextureManager>
	{
	public:
		TextureRef TextureNamed(const std::string &name);
		TextureMap textures;
			
		TextureManager();
		virtual ~TextureManager();
	};
}