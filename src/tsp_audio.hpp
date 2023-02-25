/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 16:22
 */

#pragma once

#include <dsound.h>

namespace tsp
{
    class AudioContext
    {
    public:
        AudioContext(HWND Window);
        ~AudioContext();

        void Update();

        void SetVolume(float Volume) { mVolume = Volume; }
    private:
        friend class AudioFile;
        
        IDirectSound8 *mDevice = nullptr;
        IDirectSoundBuffer *mPrimaryBuffer = nullptr;
        float mVolume;
    };
};
