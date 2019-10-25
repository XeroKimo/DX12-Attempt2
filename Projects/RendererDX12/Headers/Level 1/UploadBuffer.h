#pragma once
#include "DX12Header.h"
#include "Level 0/!Header.h"

namespace RendererDX12
{
    class UploadBuffer
    {
    public:
        UploadBuffer(BaseDevice* device, UINT64 size);
        UploadBuffer(BaseCommandList* commandList, ID3D12Resource* targetResource, D3D12_SUBRESOURCE_DATA* data);

        inline ID3D12Resource* GetInterface() { return m_resource.Get(); }
    private:
        ComPtr<ID3D12Resource> m_resource;
    };
}