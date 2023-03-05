/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 17:33
 */

#include "tsp_application.hpp"

#include <regex>
#include <iostream>
#include <fstream>

#include <imgui/imgui.h>

namespace tsp
{
    void NetworkThread(const std::shared_ptr<tsp::Network>& Network)
    {
        while (Application::GetApplication()->GetRenderer()->IsOpen()) {
            std::string Reply = Network->Receive();

            if (Reply.length() > 0) {
                std::regex Research("!(.+)@.+ PRIVMSG #([^\\s]+) :(.*)");
                std::smatch Match;
                std::regex_search(Reply, Match, Research);
                Application::GetApplication()->SendCommand(Match[3]);
            }
        }
    }

    void Application::Init()
    {
        mRenderer = std::make_shared<tsp::Renderer>();
        mGPU = std::make_shared<tsp::GPU>(mRenderer->GetWindow());
        mAudioContext = std::make_shared<tsp::AudioContext>(mRenderer->GetWindow());
        mNetwork = std::make_shared<tsp::Network>();
        mConfig = std::make_shared<tsp::Config>("config.tsp");
        mAudioRegistry = std::make_shared<tsp::AudioRegistry>();

        mAudioRegistry->Load();

        std::ifstream EnvStream("login.env");
        if (!EnvStream.is_open()) {
            MessageBoxA(nullptr, "Failed to load env file!", "Error!", MB_ICONERROR);
        }

        std::vector<std::string> Lines;
        std::string Line;
        while (std::getline(EnvStream, Line))
            Lines.push_back(Line);

        mNetwork->SetToken(Lines[0]);
        mNetwork->SetChannel(Lines[1]);
        mNetwork->Connect();

        EnvStream.close();

        mNetworkJob = std::thread(NetworkThread, mNetwork);
        mNetworkJob.detach();
    }

    void Application::Update()
    {
        while (mRenderer->IsOpen()) {
            mAudioContext->Update();
            mRenderer->Update();

            mGPU->BeginFrame();

            this->BeginDockspace();
            this->OnUI();
            this->EndDockspace();
            
            mGPU->EndFrame();
            mGPU->Present();
        }
    }

    void Application::OnUI()
    {
        ImGui::Begin("Commands");

        if (ImGui::Button("Reload")) {
            mAudioRegistry->Unload();
            mAudioRegistry->Load();
        }

        for (auto Variable = mConfig->Variables.begin(); Variable != mConfig->Variables.end(); ++Variable) {
            if (ImGui::TreeNodeEx(Variable->Name.c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
            {
                {
                    char Buffer[512] = {};
                    strcat(Buffer, Variable->Name.c_str());
                    ImGui::InputText("Name", Buffer, 512);
                    Variable->Name = Buffer;
                }
                {
                    char Buffer[512] = {};
                    strcat(Buffer, Variable->Command.c_str());
                    ImGui::InputText("Command", Buffer, 512);
                    Variable->Command = Buffer;
                }
                {
                    char Buffer[512] = {};
                    strcat(Buffer, Variable->Path.c_str());
                    ImGui::InputText("Path", Buffer, 512);
                    Variable->Path = Buffer;
                }
                ImGui::TreePop();
            }
        }

        ImGui::End();
    }

    void Application::BeginDockspace()
    {
        // Begin dockspace
        static bool DockspaceOpen = true;
        static bool FullscreenPersistant = true;
        static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (FullscreenPersistant)
        {
            ImGuiViewport* Viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(Viewport->Pos);
            ImGui::SetNextWindowSize(Viewport->Size);
            ImGui::SetNextWindowViewport(Viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &DockspaceOpen, WindowFlags);
        ImGui::PopStyleVar();

        ImGui::PopStyleVar(2);

        ImGuiIO& IO = ImGui::GetIO();
        ImGuiStyle& Style = ImGui::GetStyle();
        float MinWinSizeX = Style.WindowMinSize.x;
        Style.WindowMinSize.x = 370.0f;
        if (IO.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID DockspaceID = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);
        }

        Style.WindowMinSize.x = MinWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                    mConfig->Write("config.tsp");

                if (ImGui::MenuItem("Exit"))
                    DestroyWindow(mRenderer->GetWindow());
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    void Application::EndDockspace()
    {
        ImGui::End();
    }

    void Application::SendCommand(const std::string& Command)
    {
        mAudioRegistry->PlayCommand(Command);
    }

    void Application::Exit()
    {
        mAudioRegistry->Unload();
        mConfig->Write("config.tsp");
    }

    Application* Application::GetApplication()
    {
        static Application App;
        return &App;
    }
}
