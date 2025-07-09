// Copyright (C) 2025 iProgramInCpp
#include "PlatformPrecomp.h"
#include "AudioManagerWii.h"

//#include <asndlib.h>

#ifdef PLATFORM_WII

AudioManagerWii::AudioManagerWii()
{
	SetPreferOGG(false);
}

AudioManagerWii::~AudioManagerWii()
{
}

string AudioManagerWii::GetAudioSystemName()
{
	return "Wii Audio Manager";
}

bool AudioManagerWii::Init()
{
	//ASND_Init();
	return true;
}

void AudioManagerWii::Kill()
{
	//ASND_End();
}

AudioHandle AudioManagerWii::Play(string fName, bool bLooping, bool bIsMusic, bool bAddBasePath, bool bForceStreaming)
{
	// TODO
	return AUDIO_HANDLE_BLANK;
}

void AudioManagerWii::Preload(string fName, bool bLooping, bool bIsMusic, bool bAddBasePath, bool bForceStreaming)
{
	// TODO
}

void AudioManagerWii::KillCachedSounds(bool bKillMusic, bool bKillLooping, int ignoreSoundsUsedInLastMS, int killSoundsLowerPriorityThanThis, bool bKillSoundsPlaying)
{
	// TODO
}


void AudioManagerWii::Update()
{
	// TODO
}


void AudioManagerWii::Stop(AudioHandle soundID)
{
	// TODO
}


AudioHandle AudioManagerWii::GetMusicChannel()
{
	// TODO
	return 0;
}


bool AudioManagerWii::IsPlaying(AudioHandle soundID)
{
	// TODO
	return false;
}


void AudioManagerWii::SetMusicEnabled(bool bNew)
{
	// TODO
}


void AudioManagerWii::StopMusic()
{
	// TODO
}


void AudioManagerWii::FadeOutMusic(unsigned int duration)
{
	// TODO
}


int AudioManagerWii::GetMemoryUsed()
{
	// TODO
	return 0;
}


void AudioManagerWii::SetFrequency(AudioHandle soundID, int freq)
{
	// TODO
}


void AudioManagerWii::SetPan(AudioHandle soundID, float pan)
{
	// TODO
}

void AudioManagerWii::SetVol(AudioHandle soundID, float vol)
{
	// TODO
}


void AudioManagerWii::SetPriority(AudioHandle soundID, int priority)
{
	// TODO
}


uint32 AudioManagerWii::GetPos( AudioHandle soundID )
{
	// TODO
	return 0;
}


void AudioManagerWii::SetPos( AudioHandle soundID, uint32 posMS )
{
	// TODO
}


void AudioManagerWii::SetMusicVol(float vol)
{
	// TODO
}


void AudioManagerWii::Vibrate(int duration)
{
	// TODO
}

#endif//PLATFORM_WII
