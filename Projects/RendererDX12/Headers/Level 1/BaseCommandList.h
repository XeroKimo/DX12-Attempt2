#pragma once
#include "DX12Header.h"
#include "Level 0/!Header.h"

namespace RendererDX12
{
    class BaseCommandList
    {
    public:
        inline BaseCommandList(const BaseDevice* const device, const BaseCommandAllocator* const allocator)
        {
            device->GetInterface()->CreateCommandList(device->GetNodeMask(), allocator->GetType(), allocator->GetInterface(), nullptr, IID_PPV_ARGS(m_commandList.GetAddressOf()));
            m_device = device;
        }

        inline ID3D12GraphicsCommandList* GetInterface() const noexcept { return m_commandList.Get(); }
        inline const BaseDevice* GetDevice() const noexcept { return m_device; }
    private:
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
        const BaseDevice* m_device;
    };
}