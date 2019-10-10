#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class BaseSwapChain
    {
    public:
        BaseSwapChain(ID3D12Device* device, UINT nodeMask, ID3D12CommandQueue* commandQueue, HWND windowHandle, UINT windowWidth, UINT windowHeight);

        void ClearBackBuffer(ID3D12GraphicsCommandList* commandList);
        void ReadyBackBuffer(ID3D12GraphicsCommandList* commandList);
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