/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/02/2023 22:59
 */

#include "tsp_application.hpp"
#include "tsp_audio_registry.hpp"

namespace tsp
{
    void AudioRegistry::Load()
    {
        Application* App = Application::GetApplication();

        mAudioCommands.clear();
        for (auto Variable : App->GetConfig()->Variables) {
            AudioCommand Command;
            Command.Variable = Variable;
            Command.File.Load(Command.Variable.Path);
            mAudioCommands.push_back(Command);
        }
    }

    void AudioRegistry::Unload()
    {
        for (auto Command : mAudioCommands) {
            Command.File.Unload();
        }
    }

    void AudioRegistry::Add(tsp_variable Variable)
    {
        AudioCommand Command;
        Command.Variable = Variable;
        Command.File.Load(Command.Variable.Path);
        mAudioCommands.push_back(Command);
    }

    void AudioRegistry::PlayCommand(const std::string& Command)
    {
        for (auto Cmd : mAudioCommands) {
            if (Cmd.Variable.Command == Command) {
                Cmd.File.Play();
            }
        }
    }
}
