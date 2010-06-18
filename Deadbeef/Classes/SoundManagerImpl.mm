//
//  SoundManagerImpl.mm
//  Deadbeef
//
//  Created by Joshua Weinberg on 5/15/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SoundManagerImpl.h"
#import "SoundManager.h"

@implementation SoundManagerImpl

- (id)initWithSoundManager:(Deadbeef::SoundManager&)aManager;
{
	if ((self = [super init]))
	{
		manager = &aManager;
	}
	return self;
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag;
{
	manager->RemoveRecord(player);
}

@end
