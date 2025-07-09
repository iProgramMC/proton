// AudioManagerWii
// Copyright (C) 2025 iProgramInCpp
#ifndef AUDIO_MANAGER_WII_H
#define AUDIO_MANAGER_WII_H
#ifdef  PLATFORM_WII

#include "AudioManager.h"

class AudioManagerWii : public AudioManager
{
public:
	AudioManagerWii();
	virtual ~AudioManagerWii();
	
	virtual string GetAudioSystemName();
	virtual bool Init();
	virtual void Kill();
	
	virtual AudioHandle Play(string fName, bool bLooping = false, bool bIsMusic = false, bool bAddBasePath = true, bool bForceStreaming = false);

	virtual void Preload(string fName, bool bLooping = false, bool bIsMusic = false, bool bAddBasePath = true, bool bForceStreaming = false);

	//SoundObject * GetSoundObjectByFileName(string fName);
	virtual void KillCachedSounds(bool bKillMusic, bool bKillLooping, int ignoreSoundsUsedInLastMS, int killSoundsLowerPriorityThanThis, bool bKillSoundsPlaying);
	virtual void Update();
	virtual void Stop(AudioHandle soundID);
	virtual AudioHandle GetMusicChannel();
	virtual bool IsPlaying(AudioHandle soundID);
	virtual void SetMusicEnabled(bool bNew);
	virtual void StopMusic();
	virtual void FadeOutMusic(unsigned int duration = 1000);
	virtual int GetMemoryUsed();
	//bool DeleteSoundObjectByFileName(string fName);
	virtual void SetFrequency(AudioHandle soundID, int freq);
	virtual void SetPan(AudioHandle soundID, float pan); //0 is normal stereo, -1 is all left, +1 is all right
	virtual void SetVol(AudioHandle soundID, float vol);
	virtual void SetPriority(AudioHandle soundID, int priority);
	virtual uint32 GetPos( AudioHandle soundID );
	virtual void SetPos( AudioHandle soundID, uint32 posMS );
	virtual void SetMusicVol(float vol);
	virtual void Vibrate(int duration = 300);
};

#endif//PLATFORM_WII
#endif//AUDIO_MANAGER_WII_H
