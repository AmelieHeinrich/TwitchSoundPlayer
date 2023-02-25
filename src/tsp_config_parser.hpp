/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/02/2023 10:09
 */

#pragma once

#include <string>
#include <vector>

namespace tsp
{
    struct tsp_variable
    {
        std::string Name;
        std::string Command;
        std::string Path;        
    };

    class Config
    {
    public:
        Config(const std::string& Path);
        void Write(const std::string& Path);
        void Add(tsp_variable Variable) { Variables.push_back(Variable); }

        std::vector<tsp_variable> Variables;
    };
};