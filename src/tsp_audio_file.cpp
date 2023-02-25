/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/02/2023 16:02
 */

#include "tsp_application.hpp"
#include "tsp_audio_file.hpp"

#include <dr_libs/dr_wav.h>

namespace tsp
{
    void AudioFile::Load(const std::string& Path)
    {
        drwav Wave;
        if (!drwav_init_file(&Wave, Path.c_str(), nullptr)) {
            MessageBoxA(nullptr, "Failed to load audio file!", "Error!", 0);
        }

        mSampleRate = Wave.sampleRate;
        mSampleCount = Wave.totalPCMFrameCount;
        mChannels = Wave.channels;
        mSamples = reinterpret_cast<short*>(malloc(mSampleCount * sizeof(short) * Wave.channels));

        int ReadSamples = drwav_read_pcm_frames_s16(&Wave, mSampleCount, mSamples);
        if (ReadSamples != mSampleCount) {
            MessageBoxA(nullptr, "Failed to load the entirety of the .wav file!", "Error!", 0);
        }

        WAVEFORMATEX WaveFormat = {};
        WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
        WaveFormat.nChannels = mChannels;
        WaveFormat.nSamplesPerSec = mSampleRate;
        WaveFormat.wBitsPerSample = 16;
        WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
        WaveFormat.nAvgBytesPerSec = (WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign);

        DSBUFFERDESC BufferDesc = {};
        BufferDesc.dwSize = sizeof(DSBUFFERDESC);
        BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS;
        BufferDesc.dwBufferBytes = (DWORD)mSampleCount * WaveFormat.nChannels * sizeof(short);
        BufferDesc.lpwfxFormat = &WaveFormat;
        BufferDesc.guid3DAlgorithm = GUID_NULL;

        if (BufferDesc.dwBufferBytes > DSBSIZE_MAX) {
            MessageBoxA(nullptr, ".wav file is too big!", "Error!", 0);
        }

        IDirectSound8 *Device = Application::GetApplication()->GetAudioContext()->mDevice;
        HRESULT Result = Device->CreateSoundBuffer(&BufferDesc, &mBuffer, nullptr);
        if (FAILED(Result)) {
            MessageBoxA(nullptr, "Failed to create internal DirectSound buffer!", "Error!", 0);
        }

        LPVOID WriteVoid;
        DWORD Length;
        Result = mBuffer->Lock(0, 0, &WriteVoid, &Length, nullptr, nullptr, DSBLOCK_ENTIREBUFFER);
        if (FAILED(Result)) {
            MessageBoxA(nullptr, "Failed to upload audio file to sound ram!", "Error!", 0);
        }
        memcpy(WriteVoid, mSamples, mSampleCount * sizeof(short) * mChannels);
        mBuffer->Unlock(WriteVoid, Length, nullptr, 0);

        delete mSamples;
        drwav_uninit(&Wave);
    }

    void AudioFile::Unload()
    {
        if (mBuffer)
            mBuffer->Release();
    }

    void AudioFile::Play()
    {
        mBuffer->Play(0, 0, 0);
    }

    void AudioFile::SetVolume(float Volume)
    {
        int Range = -6000;
        Volume = (Range + Volume * 6000);

        mBuffer->SetVolume(Volume);
    }
}
