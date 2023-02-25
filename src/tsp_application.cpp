/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 17:33
 */

#include "tsp_application.hpp"

#include <regex>
#include <iostream>

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
                std::cout << Match[3] << std::endl;
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

        mNetwork->SetToken("oauth:ewtwfhl2uwrebp2imqdpremhc86lww");
        mNetwork->SetChannel("amelie_dev");
        mNetwork->Connect();

        mNetworkJob = std::thread(NetworkThread, mNetwork);
        mNetworkJob.detach();

        mTestFile.Load("sounds/meow.wav");
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
        for (auto Variable : mConfig->Variables) {
            if (ImGui::TreeNodeEx(Variable.Name.c_str(), ImGuiTreeNodeFlags_Framed))
            {
                ImGui::Text(Variable.Command.c_str());
                ImGui::Text(Variable.Path.c_str());
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
        std::cout << "RECEIVED COMAMND: " << Command << std::endl;
        if (Command.compare("!meow") == 0) {
            std::cout << "Meow" << std::endl;
            mTestFile.Play();
        }
    }

    void Application::Exit()
    {
        mTestFile.Unload();
        mConfig->Write("config.tsp");
    }

    Application* Application::GetApplication()
    {
        static Application App;
        return &App;
    }
}
