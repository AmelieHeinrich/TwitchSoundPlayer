/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 16:23
 */

#include "tsp_audio.hpp"

namespace tsp
{
    AudioContext::AudioContext(HWND Window)
    {
        HRESULT Result = DirectSoundCreate8(nullptr, &mDevice, nullptr);
        if (FAILED(Result)) {
            MessageBoxA(nullptr, "Failed to initialise audio interface!", "Error", MB_ICONERROR);
        }
        Result = mDevice->SetCooperativeLevel(Window, DSSCL_PRIORITY);
        if (FAILED(Result)) {
            MessageBoxA(nullptr, "Failed to set audio cooperative level!", "Error", MB_ICONERROR);
        }

        WAVEFORMATEX WaveFormat = {};
        WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
        WaveFormat.nChannels = 2;
        WaveFormat.nSamplesPerSec = 48000;
        WaveFormat.nBlockAlign = 4;
        WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
        WaveFormat.wBitsPerSample = 16;

        DSBUFFERDESC BufferDesc = {};
        BufferDesc.dwSize = sizeof(DSBUFFERDESC);
        BufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
        BufferDesc.dwBufferBytes = 0;
        BufferDesc.lpwfxFormat = nullptr;
        BufferDesc.guid3DAlgorithm = GUID_NULL;

        Result = mDevice->CreateSoundBuffer(&BufferDesc, &mPrimaryBuffer, nullptr);
        if (Result != DS_OK) {
            MessageBoxA(nullptr, "Failed to create primary buffer!", "Error", MB_ICONERROR);
        }
        mPrimaryBuffer->SetFormat(&WaveFormat);
    }

    AudioContext::~AudioContext()
    {
        if (mPrimaryBuffer)
            mPrimaryBuffer->Release();
        if (mDevice)
            mDevice->Release();
    }

    void AudioContext::Update()
    {
        (void)0;
    }
}
