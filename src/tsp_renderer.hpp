/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 16:36
 */

#pragma once

#include <Windows.h>

namespace tsp
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        bool IsOpen();
        void Update();

        HWND& GetWindow() { return mWindow; }
    private:
        HWND mWindow;
    };
};
