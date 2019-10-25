#pragma once
#include "DX12Header.h"
#include "Level 0/!Header.h"

namespace RendererDX12
{
    class BaseSwapChain
    {
    public:
        BaseSwapChain(BaseDevice* device, ID3D12CommandQueue* commandQueue, HWND windowHandle, UINT windowWidth, UINT windowHeight);

        void ClearBackBuffer(BaseCommandList* commandList);
        void ReadyBackBuffer(BaseCommandList* commandList);
        inline IDXGISwapChain3* GetInterface() { return m_swapChain.Get(); }
    private:
        ComPtr<IDXGISwapChain3> m_swapChain;
        ComPtr<ID3D12DescriptorHeap> m_renderTargetHeap;
        std::vector<ComPtr<ID3D12Resource>> m_frameBuffers;
        D3D12_VIEWPORT m_viewPort;
        D3D12_RECT m_rect;
        UINT m_descriptorHeapSize;
    };
}