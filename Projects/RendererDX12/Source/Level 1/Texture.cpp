#include "RendererDX12.h"
#include <wincodec.h>

namespace RendererDX12
{
    using namespace Helpers;
    shared_ptr<UploadBuffer> Texture::InitializeTexture2D(BaseCommandList* commandList, TextureData textureData)
    {
        ID3D12Device* device = commandList->GetDevice()->GetInterface();
        device->CreateCommittedResource(&HeapDefault(0), D3D12_HEAP_FLAG_NONE, &ResourceTexture2D(textureData.imageWidth, textureData.imageHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 1), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));

        D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
        heapDesc.NodeMask = 0;
        heapDesc.NumDescriptors = 1;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_heap.GetAddressOf()));
        assert(SUCCEEDED(hr));

        m_resourceView.Format = m_resource.Get()->GetDesc().Format;
        m_resourceView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        m_resourceView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        m_resourceView.Texture2D.MipLevels = 1;
        m_resourceView.Texture2D.MostDetailedMip = 0;
        m_resourceView.Texture2D.PlaneSlice = 0;
        m_resourceView.Texture2D.ResourceMinLODClamp = 0.0f;

        device->CreateShaderResourceView(m_resource.Get(), &m_resourceView, m_heap->GetCPUDescriptorHandleForHeapStart());
        D3D12_SUBRESOURCE_DATA data;
        data.pData = textureData.imageData.get();
        data.RowPitch = static_cast<LONG_PTR>(textureData.imageWidth) * 4;
        data.SlicePitch = textureData.imageHeight * data.RowPitch;

        shared_ptr<UploadBuffer> uploadBuffer = make_shared <UploadBuffer>(commandList, m_resource.Get(), &data);

        commandList->GetInterface()->ResourceBarrier(1, &ResourceBarrierTransition(m_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));

        return uploadBuffer;
    }

    void Texture::SetForDraw(BaseCommandList* commandList, const UINT& paramIndex)
    {
        commandList->GetInterface()->SetDescriptorHeaps(1, m_heap.GetAddressOf());
        commandList->GetInterface()->SetGraphicsRootDescriptorTable(paramIndex, m_heap->GetGPUDescriptorHandleForHeapStart());
    }
}