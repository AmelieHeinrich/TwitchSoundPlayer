/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/02/2023 16:01
 */

#pragma once

#include <dsound.h>
#include <string>

namespace tsp
{
    class AudioFile
    {
    public:
        void Load(const std::string& Path);
        void Unload();
        void Play();
        void SetVolume(float Volume);

    private:
        int mSampleRate;
        int mChannels;
        int mSampleCount;
        short *mSamples;

        IDirectSoundBuffer *mBuffer;
    };
};
