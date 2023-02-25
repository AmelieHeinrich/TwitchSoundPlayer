/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/02/2023 10:15
 */

#include "tsp_config_parser.hpp"

#include <fstream>
#include <Windows.h>
#include <string>


namespace tsp
{
    std::string GetSubstringBefore(const std::string& String, char Character)
    {
        std::string::size_type Position = String.find(Character);
        if (Position != std::string::npos)
            return String.substr(0, Position);
        else
            return String;
    }

    std::string GetSubstringAfter(const std::string& String, char Character)
    {
        std::string::size_type Position = String.find(Character);
        if (Position != std::string::npos)
            return String.substr(Position + 1, String.length());
        else
            return String;
    }

    Config::Config(const std::string& Path)
    {
        // Meow=!meow,sounds/meow.wav
        // Name=command,path
        std::ifstream Stream(Path);
        if (!Stream.is_open()) {
            MessageBoxA(nullptr, "Failed to open config file!", "Error!", MB_ICONERROR);
        }

        std::string Line;
        while (std::getline(Stream, Line))
        {
            std::string Name = GetSubstringBefore(Line, '=');
            std::string Command = GetSubstringBefore(GetSubstringAfter(Line, '='), ',');
            std::string Path = GetSubstringAfter(Line, ',');

            tsp_variable Variable = { Name, Command, Path };
            Variables.push_back(Variable);
        }

        Stream.close();
    }

    void Config::Write(const std::string& Path)
    {
        std::ofstream Stream(Path, std::ios_base::trunc);
        for (auto Variable : Variables) {
            Stream << Variable.Name << "=" << Variable.Command << "," << Variable.Path << std::endl;
        }
        Stream.close();
    }
}
