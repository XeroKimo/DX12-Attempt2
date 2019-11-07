#pragma once
#include "DX12Header.h"
#include "Level 0/!Header.h"

namespace RendererDX12
{
    class BaseSwapChain
    {
    public:
        BaseSwapChain(BaseDevice* const device, ID3D12CommandQueue* const commandQueue, const HWND& windowHandle, const UINT& windowWidth, const UINT& windowHeight);

        void ClearBackBuffer(const BaseCommandList* const commandList) const;
        void ReadyBackBuffer(const BaseCommandList* const commandList) const;
        inline IDXGISwapChain3* GetInterface() const noexcept { return m_swapChain.Get(); }

        void ResizeSwapChain();
    private:
        ComPtr<IDXGISwapChain3> m_swapChain;
        ComPtr<ID3D12DescriptorHeap> m_renderTargetHeap;
        std::vector<ComPtr<ID3D12Resource>> m_frameBuffers;
        ComPtr<ID3D12DescriptorHeap> m_depthStencilHeap;
        ComPtr<ID3D12Resource> m_depthStencil;
        BaseDevice* m_device;
        D3D12_VIEWPORT m_viewPort;
        D3D12_RECT m_rect;
        UINT m_descriptorHeapSize;
    };
}