/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 16:38
 */

#include "renderer.hpp"

namespace tsp
{
    LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
    {
        return DefWindowProc(Window, Message, WParam, LParam);
    }

    Renderer::Renderer()
    {
        WNDCLASSA WindowClass = {};
        WindowClass.lpszClassName = "TSPWindowClass";
        WindowClass.lpfnWndProc = WindowProc;
        WindowClass.hInstance = GetModuleHandle(nullptr);
        WindowClass.hCursor = LoadCursor(WindowClass.hInstance, IDC_ARROW);
        RegisterClassA(&WindowClass);

        mWindow = CreateWindowA(WindowClass.lpszClassName, "Twitch Sound Player | <Direct3D 11>", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, nullptr, nullptr, WindowClass.hInstance, nullptr);
        ShowWindow(mWindow, SW_SHOWDEFAULT);
    }

    Renderer::~Renderer()
    {
        DestroyWindow(mWindow);
    }

    bool Renderer::IsOpen()
    {
        return IsWindowVisible(mWindow);
    }

    void Renderer::Update()
    {
        MSG Message;
        while (PeekMessageA(&Message, mWindow, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
    }
}
