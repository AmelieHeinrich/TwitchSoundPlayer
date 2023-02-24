/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 16:38
 */

#include "tsp_application.hpp"
#include "tsp_renderer.hpp"

#include <cstdint>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace tsp
{
    LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(Window, Message, WParam, LParam))
            return 1;

        switch (Message)
        {
            case WM_SIZE:
            {
                RECT Rectangle;
                GetClientRect(Window, &Rectangle);
                uint32_t Width = Rectangle.right - Rectangle.left;
                uint32_t Height = Rectangle.bottom - Rectangle.top;

                Application* App = Application::GetApplication();
                if (App->GetGPU())
                    App->GetGPU()->Resize(Width, Height);
            } break;
        }

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
