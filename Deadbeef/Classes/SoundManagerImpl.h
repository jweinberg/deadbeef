//
//  SoundManagerImpl.h
//  Deadbeef
//
//  Created by Joshua Weinberg on 5/15/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
#import "SoundManager.h"

@interface SoundManagerImpl : NSObject <AVAudioPlayerDelegate>
{
	Deadbeef::SoundManager * manager;
}

- (id)initWithSoundManager:(Deadbeef::SoundManager&)aManager;
@end