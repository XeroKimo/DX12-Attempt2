#include "RendererDX12.h"

namespace RendererDX12
{
    using namespace Helpers;
    UploadBuffer::UploadBuffer(BaseDevice* device, UINT64 size)
    {
        HRESULT hr = device->GetInterface()->CreateCommittedResource(&HeapUpload(device->GetNodeMask()), D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES, &ResourceBuffer(size), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
        assert(SUCCEEDED(hr));
    }

    UploadBuffer::UploadBuffer(const BaseCommandList* const commandList, ID3D12Resource* const targetResource, const D3D12_SUBRESOURCE_DATA* const data)
    {
        const BaseDevice* device = commandList->GetDevice();
        UINT64 sizeRequired;
        device->GetInterface()->GetCopyableFootprints(&targetResource->GetDesc(), 0, 1, 0, nullptr, nullptr, nullptr, &sizeRequired);

        device->GetInterface()->CreateCommittedResource(&HeapUpload(device->GetNodeMask()), D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES, &ResourceBuffer(sizeRequired), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
        UpdateSubresources(commandList->GetInterface(), targetResource, m_resource.Get(), 0, 0, 1, data);
    }
}