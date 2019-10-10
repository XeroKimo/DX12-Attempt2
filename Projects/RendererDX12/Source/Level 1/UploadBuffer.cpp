#include "RendererDX12.h"

namespace RendererDX12
{
    using namespace Helpers;
    UploadBuffer::UploadBuffer(ID3D12Device* device, UINT nodeMask, UINT64 size)
    {
        HRESULT hr = device->CreateCommittedResource(&HeapUpload(nodeMask), D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES, &ResourceBuffer(size), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
        assert(SUCCEEDED(hr));

        D3D12_RANGE range = { 0,0 };
        m_resource.Get()->Map(0, &range, reinterpret_cast<void**>(&m_mappedAddress));

        m_totalSize = size;
    }

    UploadBuffer::UploadBuffer(ID3D12GraphicsCommandList* commandList, UINT nodeMask, ID3D12Resource* targetResource, D3D12_SUBRESOURCE_DATA* data) :
        m_mappedAddress(nullptr),
        m_totalSize(0),
        m_usedSize(0)
    {
        ComPtr<ID3D12Device> device;
        commandList->GetDevice(IID_PPV_ARGS(device.GetAddressOf()));

        device->GetCopyableFootprints(&targetResource->GetDesc(), 0, 1, 0, nullptr, nullptr, nullptr, &m_totalSize);

        device->CreateCommittedResource(&HeapUpload(nodeMask), D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES, &ResourceBuffer(m_totalSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
        UpdateSubresources(commandList, targetResource, m_resource.Get(), 0, 0, 1, data);
    }

    D3D12_GPU_VIRTUAL_ADDRESS UploadBuffer::UploadCBVSRVUAV(void* data, UINT64 size)
    {
        D3D12_GPU_VIRTUAL_ADDRESS address = m_resource.Get()->GetGPUVirtualAddress() + m_usedSize;
        memcpy(m_mappedAddress + m_usedSize, data, static_cast<size_t>(size));
        m_usedSize += (static_cast<size_t>(size) + ConstBufferSize - 1) & ~(ConstBufferSize - 1);
        return address;
    }

    void UploadBuffer::Reset()
    {
        ZeroMemory(m_mappedAddress, static_cast<size_t>(m_totalSize));
        m_usedSize = 0;
    }
}