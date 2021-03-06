#pragma once

#include "Sound.hpp"
#include "SoundSDL.hpp"
#include <mutex>

#define MAX_VOICE_COUNT 1024
extern std::atomic<SDLSoundBuffer*> sAE_ActiveVoices[MAX_VOICE_COUNT];


enum SDLSoundBufferStatus
{
    Stopped = 0,
    Paused = 1,
    Playing = 2,
};

class SDLSoundBuffer final
{
public:
    SDLSoundBuffer(const SDLSoundBuffer& rhs);

    SDLSoundBuffer& operator=(const SDLSoundBuffer& rhs);

    SDLSoundBuffer();


    SDLSoundBuffer(const DSBUFFERDESC& bufferDesc, s32 soundSysFreq);


    HRESULT SetVolume(s32 volume);
    HRESULT Play(s32 /*reserved*/, s32 /*priority*/, s32 flags);
    HRESULT Stop();

    HRESULT SetFrequency(s32 frequency);
    HRESULT SetCurrentPosition(s32 position);
    HRESULT GetCurrentPosition(u32* readPos, u32* writePos);
    HRESULT GetFrequency(u32* freq);
    HRESULT SetPan(s32 pan);
    void Release();
    HRESULT GetStatus(u32* r);

    void Destroy();

    std::vector<u8>* GetBuffer();
    void Duplicate(SDLSoundBuffer** dupePtr);

public:
    struct AE_SDL_Voice_State final
    {
        s32 iVolume;
        bool bVolDirty;
        s32 iVolumeTarget;
        f32 fFrequency;
        s32 iPan;

        SDLSoundBufferStatus eStatus;
        bool bLoop;
        bool bIsReleased;
        f32 fPlaybackPosition;

        s32 iSampleCount;
        s32 iChannels;
        s32 iBlockAlign;
    };

    s32 mSoundSysFreq = 0;
    AE_SDL_Voice_State mState;
    std::shared_ptr<std::vector<u8>> mBuffer;
    std::mutex mLock;
};
