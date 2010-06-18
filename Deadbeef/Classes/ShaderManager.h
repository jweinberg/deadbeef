/*
 *  ShaderManager.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Singleton.h"
#include <string>
#include <map>
#include "Shader.h"

namespace Deadbeef
{
	class Shader;
	
	typedef std::map<std::string, ShaderRef> ShaderMap;
	
	class ShaderManager : public Singleton<ShaderManager>
	{
	public:
		ShaderManager();
		virtual ~ShaderManager();
		
		ShaderRef ShaderNamed(const std::string &name);
	private:
		ShaderMap shaders;
	};
}