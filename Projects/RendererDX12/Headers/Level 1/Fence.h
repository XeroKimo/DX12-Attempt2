#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    const UINT64 FENCE_MAX_SIGNAL_VALUE = UINT64_MAX;
    class Fence
    {
    public:
        Fence(ID3D12Device* device, D3D12_FENCE_FLAGS flags = D3D12_FENCE_FLAG_NONE)
        {
            device->CreateFence(0, flags, IID_PPV_ARGS(m_fence.GetAddressOf()));
        }

        void Reset()
        {
            fenceValue = highestSyncedValue = 0;
            m_fence->Signal(fenceValue);
        }

        inline ID3D12Fence* GetInterface() { return m_fence.Get(); }
    public:
        UINT64 fenceValue = 0;
        UINT64 highestSyncedValue = 0;

    private:
        ComPtr<ID3D12Fence> m_fence;
    };
}