/*
 *  AtlasManager.mm
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AtlasManager.h"
#import "AtlasManagerImpl.h"
namespace Deadbeef
{
	AtlasManager::AtlasManager()
	{
		manager = [[AtlasManagerImpl alloc] init];
	}

	AtlasManager::~AtlasManager()
	{
		[manager release];
	}
	
	bool AtlasManager::TextureAndUVsNamed(const std::string& name, TextureRef& texture, CGRect& rect)
	{
		bool ret = [manager texture:texture andSourceRect:rect forSpriteNamed:[NSString stringWithUTF8String:name.c_str()]];
        return ret;
	}
	
	std::vector<std::string> AtlasManager::AnimationFrames(const std::string& name)
	{
		std::vector<std::string> frames;
		NSArray *names = [manager framesForAnimationNamed:[NSString stringWithUTF8String:name.c_str()]];
		for (NSString * str in names)
		{
			frames.push_back([str UTF8String]);
		}
		return frames;
	}
	
}
