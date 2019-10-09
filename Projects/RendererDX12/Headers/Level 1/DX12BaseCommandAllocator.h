#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class DX12BaseCommandAllocator
    {
    public:
        inline DX12BaseCommandAllocator(ID3D12Device* device, const Command_List_Type& type)
        {
            m_type = type;
            HRESULT hr = device->CreateCommandAllocator(static_cast<const D3D12_COMMAND_LIST_TYPE>(type), IID_PPV_ARGS(m_allocator.GetAddressOf()));
            assert(SUCCEEDED(hr));
        }

        inline ID3D12CommandAllocator* GetInterface() { return m_allocator.Get(); }
        inline Command_List_Type GetType() { return m_type; }
    private:
        ComPtr<ID3D12CommandAllocator> m_allocator;
        Command_List_Type m_type = Command_List_Type::Direct;
    };
}