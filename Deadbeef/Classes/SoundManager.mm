/*
 *  SoundManager.mm
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/8/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoundManager.h"
#import <AVFoundation/AVFoundation.h>
#import "SoundManagerImpl.h"

#define MAX_SOUNDS (10)

namespace Deadbeef
{
	SoundRecord::SoundRecord(const std::string& sound, float volume, bool loop)
	{
		NSString * path = [NSString stringWithUTF8String:sound.c_str()];
		NSURL * url = [[NSURL alloc] initFileURLWithPath:[[NSBundle mainBundle] pathForResource:path ofType:@"caf"]];
		audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:nil];
		[(id)audioPlayer setVolume:volume];
		if (loop)
			[(id)audioPlayer setNumberOfLoops:-1];
		[url release];
	}

	SoundRecord::~SoundRecord()
	{
		[(id)audioPlayer release];
	}

	void SoundRecord::Play()
	{
		[NSThread detachNewThreadSelector:@selector(play) toTarget:(id)audioPlayer withObject:nil];	
	}

	void SoundRecord::Stop()
	{
		[(id)audioPlayer stop];
	}

	SoundManager::SoundManager()
	{
		managerImpl = [[SoundManagerImpl alloc] initWithSoundManager:*this];
	}

	SoundManager::~SoundManager()
	{
		[(id)managerImpl release];
	}

	SoundRecordRef SoundManager::PlaySound(const std::string& name, float volume, bool loop)
	{
		if (soundsPlaying >= MAX_SOUNDS)
			return SoundRecordRef();
		
		SoundRecordRef ref = SoundRecordRef(new SoundRecord(name, volume, loop));
		records.push_back(ref);
		[(id)ref->GetAudioPlayer() setDelegate:(id)managerImpl];
		ref->Play();	
		soundsPlaying++;
		return ref;
	}
	
	void SoundManager::RemoveRecord(void * player)
	{
		SoundRecordList::iterator it = records.begin();
		for (;it != records.end(); ++it)
		{
			if ((*it)->GetAudioPlayer() == player)
			{
				soundsPlaying--;
				debug_log("Killed sound");
				records.erase(it);
				break;
			}
		}
	}

}
