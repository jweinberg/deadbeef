/*
 *  SoundManager.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 5/8/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "Singleton.h"
#include <tr1/memory>
#include <string>
#include <vector>

namespace Deadbeef
{
	class SoundRecord
	{
	public:
		SoundRecord(const std::string& sound, float volume, bool loop);
		virtual ~SoundRecord();
		const void * GetAudioPlayer() { return audioPlayer; }
		void Play();
		void Stop();
	private:
		void * audioPlayer;
	};

	typedef std::tr1::shared_ptr<SoundRecord> SoundRecordRef;
	typedef std::tr1::weak_ptr<SoundRecord> SoundRecordWeakRef;

	typedef std::vector<SoundRecordRef> SoundRecordList;

	class SoundManager : public Singleton<SoundManager>
	{
	public:
		SoundManager();
		virtual ~SoundManager();
		SoundRecordRef PlaySound(const std::string& name, float volume, bool loop);
		void RemoveRecord(void * player);
	private:
		int soundsPlaying;
		SoundRecordList records;
		void * managerImpl;
	};
}

