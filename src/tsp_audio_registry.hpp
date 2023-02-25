/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/02/2023 22:56
 */

#pragma once

#include "tsp_audio_file.hpp"
#include "tsp_config_parser.hpp"

#include <vector>
#include <memory>

namespace tsp
{
    struct AudioCommand
    {
        AudioFile File;
        tsp_variable Variable;
    };

    class AudioRegistry
    {
    public:
        void Load();
        void Unload();
        void Add(tsp_variable Variable);
        void PlayCommand(const std::string& Command);
    private:
        std::vector<AudioCommand> mAudioCommands;
    };
};
