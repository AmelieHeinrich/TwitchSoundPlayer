/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 17:33
 */

#include "tsp_application.hpp"

#include <regex>
#include <iostream>

namespace tsp
{
    void NetworkThread(const std::shared_ptr<tsp::Network>& Network)
    {
        std::string Reply = Network->Receive();

        if (Reply.length() > 0) {
            std::regex Research("!(.+)@.+ PRIVMSG #([^\\s]+) :(.*)");
            std::smatch Match;
            if (std::regex_search(Reply, Match, Research) == true)
                std::cout << Match[1] << ": " << Match[3] << std::endl;
        }
    }

    void Application::Init()
    {
        mRenderer = std::make_shared<tsp::Renderer>();
        mGPU = std::make_shared<tsp::GPU>(mRenderer->GetWindow());
        mAudioContext = std::make_shared<tsp::AudioContext>(mRenderer->GetWindow());
        mNetwork = std::make_shared<tsp::Network>();

        mNetwork->SetToken("oauth:ewtwfhl2uwrebp2imqdpremhc86lww");
        mNetwork->SetChannel("amelie_dev");
        mNetwork->Connect();

        mNetworkJob = std::thread(NetworkThread, mNetwork);
    }

    void Application::Update()
    {
        while (mRenderer->IsOpen()) {
            mAudioContext->Update();
            mRenderer->Update();

            mGPU->BeginFrame();
            mGPU->EndFrame();
            mGPU->Present();
        }
    }

    void Application::Exit()
    {
        mNetworkJob.join();
    }

    Application* Application::GetApplication()
    {
        static Application App;
        return &App;
    }
}
