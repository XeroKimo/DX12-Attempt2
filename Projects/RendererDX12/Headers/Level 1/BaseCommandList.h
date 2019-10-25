#pragma once
#include "DX12Header.h"
#include "Level 0/!Header.h"

namespace RendererDX12
{
    class BaseCommandList
    {
    public:
        inline BaseCommandList(BaseDevice* device, BaseCommandAllocator* allocator)
        {
            device->GetInterface()->CreateCommandList(device->GetNodeMask(), allocator->GetType(), allocator->GetInterface(), nullptr, IID_PPV_ARGS(m_commandList.GetAddressOf()));
            m_device = device;
        }

        inline ID3D12GraphicsCommandList* GetInterface() { return m_commandList.Get(); }
        inline BaseDevice* GetDevice() { return m_device; }
    private:
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
        BaseDevice* m_device;
    };
}