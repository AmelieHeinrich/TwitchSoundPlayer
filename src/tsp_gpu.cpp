/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 17:02
 */

#include "tsp_gpu.hpp"

namespace tsp
{
    const D3D_DRIVER_TYPE DriverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    GPU::GPU(HWND Window)
    {
        mRenderWindow = Window;
        
        D3D_FEATURE_LEVEL Levels[] = { D3D_FEATURE_LEVEL_11_0 };
        D3D_FEATURE_LEVEL TargetLevel;
        HRESULT Result = 0;
        for (int DriverIndex = 0; DriverIndex < ARRAYSIZE(DriverTypes); DriverIndex++){
            Result = D3D11CreateDevice(nullptr, DriverTypes[DriverIndex], nullptr, 0, Levels, 1, D3D11_SDK_VERSION, &mDevice, &TargetLevel, &mDeviceContext);

            if (SUCCEEDED(Result))
                break;
        }

        if (FAILED(Result)) {
            MessageBoxA(nullptr, "Failed to create D3D11 device!", "Error!", MB_ICONERROR);
        }

        mDevice->QueryInterface(IID_PPV_ARGS(&mDXGI));
        mDXGI->GetParent(IID_PPV_ARGS(&mAdapter));
        mAdapter->GetParent(IID_PPV_ARGS(&mFactory));

        RECT Rect;
        GetClientRect(mRenderWindow, &Rect);
        mWidth = Rect.right - Rect.left;
        mHeight = Rect.bottom - Rect.top;
        Resize(mWidth, mHeight);
    }

    GPU::~GPU()
    {
        if (mRenderTargetView)
            mRenderTargetView->Release();
        if (mSwapChainBuffer)
            mSwapChainBuffer->Release();
        if (mSwapChain)
            mSwapChain->Release();
        if (mFactory)
            mFactory->Release();
        if (mAdapter)
            mAdapter->Release();
        if (mDXGI)
            mDXGI->Release();
        if (mDeviceContext)
            mDeviceContext->Release();
        if (mDevice)
            mDevice->Release();
    }

    void GPU::Present()
    {
        mSwapChain->Present(1, 0);
    }

    void GPU::Resize(uint32_t Width, uint32_t Height)
    {
        mWidth = Width;
        mHeight = Height;

        if (!mSwapChain)
        {
            DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
            SwapChainDesc.BufferDesc.Width = mWidth;
            SwapChainDesc.BufferDesc.Height = mHeight;
            SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
            SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
            SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            SwapChainDesc.SampleDesc.Count = 1;		
            SwapChainDesc.SampleDesc.Quality = 0;	
            SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            SwapChainDesc.BufferCount = 1;
            SwapChainDesc.OutputWindow = mRenderWindow;
            SwapChainDesc.Flags = 0;
            SwapChainDesc.Windowed = TRUE;

            HRESULT result = mFactory->CreateSwapChain(mDevice, &SwapChainDesc, &mSwapChain);
            if (FAILED(result)) {
                MessageBoxA(nullptr, "Failed to create D3D11 swap chain!", "Error!", MB_ICONERROR);
            }
        }

        if (mRenderTargetView)
            mRenderTargetView->Release();
        if (mSwapChainBuffer)
            mSwapChainBuffer->Release();

        mSwapChain->ResizeBuffers(1, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        mSwapChain->GetBuffer(0, IID_PPV_ARGS(&mSwapChainBuffer));
        mDevice->CreateRenderTargetView(mSwapChainBuffer, NULL, &mRenderTargetView);
    }

    void GPU::BeginFrame()
    {
        D3D11_VIEWPORT Viewport;
        ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
        Viewport.TopLeftX = 0;
        Viewport.TopLeftY = 0;
        Viewport.Width = (FLOAT)mWidth;
        Viewport.Height = (FLOAT)mHeight;
        Viewport.MinDepth = 0.0f;
        Viewport.MaxDepth = 1.0f;

        mDeviceContext->RSSetViewports(1, &Viewport);
        mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, NULL);
    }

    void GPU::EndFrame()
    {
        return;
    }
}
