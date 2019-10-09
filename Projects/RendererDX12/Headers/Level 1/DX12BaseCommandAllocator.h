#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class DX12BaseCommandAllocator
    {
    public:
        inline DX12BaseCommandAllocator(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type)
        {
            m_type = type;
            HRESULT hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(m_allocator.GetAddressOf()));
            assert(SUCCEEDED(hr));
        }

        inline ID3D12CommandAllocator* GetInterface() { return m_allocator.Get(); }
        inline D3D12_COMMAND_LIST_TYPE GetType() { return m_type; }
    private:
        ComPtr<ID3D12CommandAllocator> m_allocator;
        D3D12_COMMAND_LIST_TYPE m_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    };
}