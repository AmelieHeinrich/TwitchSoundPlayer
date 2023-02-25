/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 17:28
 */

#pragma once

#include <memory>
#include <thread>

#include "tsp_network.hpp"
#include "tsp_audio.hpp"
#include "tsp_audio_file.hpp"
#include "tsp_renderer.hpp"
#include "tsp_gpu.hpp"
#include "tsp_config_parser.hpp"

namespace tsp
{
    class Application
    {
    public:
        void Init();
        void Exit();
        void Update();
        void SendCommand(const std::string& Command);

        auto GetRenderer() { return mRenderer; }
        auto GetGPU() { return mGPU; }
        auto GetAudioContext() { return mAudioContext; }
        auto GetNetwork() { return mNetwork; }

        static Application* GetApplication();
    private:
        std::shared_ptr<Renderer> mRenderer;
        std::shared_ptr<GPU> mGPU;
        std::shared_ptr<AudioContext> mAudioContext;
        std::shared_ptr<Network> mNetwork;
        std::shared_ptr<Config> mConfig;
        std::thread mNetworkJob;

        AudioFile mTestFile;

        void BeginDockspace();
        void OnUI();
        void EndDockspace();
    };
};
