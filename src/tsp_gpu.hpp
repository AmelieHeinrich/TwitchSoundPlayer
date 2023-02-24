/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 17:01
 */

#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <cstdint>

namespace tsp
{
    class GPU
    {
    public:
        GPU(HWND Window);
        ~GPU();

        void Present();
        void Resize(uint32_t Width, uint32_t Height);

        void BeginFrame();
        void EndFrame();
    private:
        HWND mRenderWindow;
        ID3D11Device *mDevice = nullptr;
        ID3D11DeviceContext *mDeviceContext = nullptr;
        IDXGIDevice *mDXGI = nullptr;
        IDXGIFactory *mFactory = nullptr;
        IDXGIAdapter *mAdapter = nullptr;
        IDXGISwapChain *mSwapChain = nullptr;
        ID3D11Texture2D *mSwapChainBuffer = nullptr;
        ID3D11RenderTargetView *mRenderTargetView = nullptr;

        uint32_t mWidth, mHeight;
    };
};
