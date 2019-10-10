#include "RendererDX12.h"

namespace RendererDX12
{
    using namespace Helpers;
    Mesh::Mesh() :
        m_indexCount(0),
        m_vertexCount(0),
        m_vertexView(),
        m_indexView()
    {
    }

    void Mesh::CreateVertexBuffer(CommandList* commandList, void* vertexData, UINT sizeOfVertex, UINT vertexCount)
    {
        ComPtr<ID3D12Device> device;
        HRESULT hr = commandList->GetBase()->GetInterface()->GetDevice(IID_PPV_ARGS(device.GetAddressOf()));
        assert(SUCCEEDED(hr));

        LONG_PTR totalVertexSize = static_cast<LONG_PTR>(sizeOfVertex)* static_cast<LONG_PTR>(vertexCount);

        device->CreateCommittedResource(&HeapDefault(0), D3D12_HEAP_FLAG_NONE, &ResourceBuffer(totalVertexSize), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_vertexBuffer.GetAddressOf()));

        D3D12_SUBRESOURCE_DATA data;
        data.pData = vertexData;
        data.RowPitch = sizeOfVertex;
        data.SlicePitch = data.RowPitch * vertexCount;

        commandList->UploadData(m_vertexBuffer.Get(), &data);
        commandList->GetBase()->GetInterface()->ResourceBarrier(1, &ResourceBarrierTransition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));

        m_vertexView.BufferLocation = m_vertexBuffer.Get()->GetGPUVirtualAddress();
        m_vertexView.SizeInBytes = static_cast<UINT>(totalVertexSize);
        m_vertexView.StrideInBytes = static_cast<UINT>(sizeOfVertex);

        m_vertexCount = vertexCount;
    }

    void Mesh::Set(CommandList* commandList)
    {
        commandList->GetBase()->GetInterface()->IASetVertexBuffers(0, 1, &m_vertexView);
    }

    void Mesh::Draw(CommandList* commandList)
    {
        commandList->GetBase()->GetInterface()->DrawInstanced(m_vertexCount, 1, 0, 0);
    }
}