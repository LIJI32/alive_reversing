#include "stdafx.h"
#include "Reverb.hpp"

#if USE_SDL2_SOUND

    #include <math.h>
    #include <iostream>
    #include <memory>
    #include <vector>

// Reference https://www.vegardno.net/2016/05/writing-reverb-filter-from-first.html

const s32 ReverbEchos = 24;
static StereoSample_S16 sReverbBuffer[1024 * 32] = {};
const f32 gReverbMix = 1.0f / ReverbEchos;

class FeedbackBuffer final
{
public:
    explicit FeedbackBuffer(s32 samples)
        : mSamples(samples)
    {
        mBuffer.resize(samples);
        for (auto& sample : mBuffer)
        {
            sample.left = 0;
            sample.right = 0;
        }
    }

    void PushSample(StereoSample_S16 s)
    {
        mBuffer[mIdx].left += s.left;
        mBuffer[mIdx].right += s.right;

        mIdx++;

        if (mIdx >= mSamples)
        {
            mIdx = 0;
        }
    }

    StereoSample_S32 GetSample()
    {
        const StereoSample_S32 f = mBuffer[mIdx];
        mBuffer[mIdx].left -= mBuffer[mIdx].left / 10;
        mBuffer[mIdx].right -= mBuffer[mIdx].right / 10;
        return f;
    }

    s32 mIdx = 0;
    s32 mSamples;
    std::vector<StereoSample_S32> mBuffer;
};

static std::vector<std::unique_ptr<FeedbackBuffer>> gFeedbackBuffers;

void Reverb_Init(s32 sampleRate)
{
    const s32 sampleGap = sampleRate / 800;
    for (s32 i = 1; i <= ReverbEchos; i++)
    {
        gFeedbackBuffers.emplace_back(new FeedbackBuffer((sampleGap + (i * 2)) * i)); // make_unique not in all supports cpp stdlibs
    }
}

void Reverb_DeInit()
{
    gFeedbackBuffers.clear();
}

void Reverb_PushSample(StereoSample_S16 v)
{
    for (auto& buffer : gFeedbackBuffers)
    {
        buffer->PushSample(v);
    }
}

inline void Reverb_Update(s32 index)
{
    sReverbBuffer[index].left = 0;
    sReverbBuffer[index].right = 0;

    for (auto& buffer : gFeedbackBuffers)
    {
        const StereoSample_S32 v = buffer->GetSample();

        sReverbBuffer[index].left += static_cast<s16>(v.left * gReverbMix);
        sReverbBuffer[index].right += static_cast<s16>(v.right * gReverbMix);
    }
}

void Reverb_Mix(StereoSample_S16* dst, SDL_AudioFormat format, Uint32 len, s32 volume)
{
    for (u32 i = 0; i < len / sizeof(StereoSample_S16); i++)
    {
        Reverb_PushSample(dst[i]);
        Reverb_Update(i);
    }

    SDL_MixAudioFormat(reinterpret_cast<Uint8*>(dst), reinterpret_cast<Uint8*>(sReverbBuffer), format, len, volume);
    // memcpy(dst, sReverbBuffer, len); // Uncomment to hear only reverb
}

#endif
