/*
 *  AtlasManager.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "Singleton.h"
#import "Texture.h"
#include <vector>

#ifdef __OBJC__
@class AtlasManagerImpl;
#else
class AtlasManagerImpl;
#endif

namespace Deadbeef
{
	class AtlasManager : public Singleton<AtlasManager>
	{
	public:
		
		AtlasManager();
		virtual ~AtlasManager();
		
		bool TextureAndUVsNamed(const std::string& name, TextureRef& texture, CGRect& rect);
		std::vector<std::string> AnimationFrames(const std::string& name);
	private:
		AtlasManagerImpl *manager;
	};
}
