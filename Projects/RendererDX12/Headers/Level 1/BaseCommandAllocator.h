#pragma once
#include "DX12Header.h"
#include "Level 0/!Header.h"

namespace RendererDX12
{
    class BaseCommandAllocator
    {
    public:
        inline BaseCommandAllocator(const BaseDevice* const device, const D3D12_COMMAND_LIST_TYPE& type)
        {
            m_type = type;
            HRESULT hr = device->GetInterface()->CreateCommandAllocator(type, IID_PPV_ARGS(m_allocator.GetAddressOf()));
            assert(SUCCEEDED(hr));
        }

        inline ID3D12CommandAllocator* GetInterface() const noexcept { return m_allocator.Get(); }
        inline D3D12_COMMAND_LIST_TYPE GetType() const noexcept { return m_type; }
    private:
        ComPtr<ID3D12CommandAllocator> m_allocator;
        D3D12_COMMAND_LIST_TYPE m_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    };
}