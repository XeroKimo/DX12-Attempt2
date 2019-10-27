#include "RendererDX12.h"

namespace RendererDX12
{
    using namespace Helpers;
    Mesh::Mesh() :
        m_indexCount(0),
        m_vertexCount(0),
        m_vertexView(),
        m_indexView(),
        m_defaultTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    {
    }

    shared_ptr<UploadBuffer> Mesh::CreateVertexBuffer(const BaseCommandList* const commandList, const void* const vertexData, const UINT& sizeOfVertexType, const UINT& vertexCount)
    {
        LONG_PTR totalVertexSize = static_cast<LONG_PTR>(sizeOfVertexType) * static_cast<LONG_PTR>(vertexCount);

        const BaseDevice* device = commandList->GetDevice();
        device->GetInterface()->CreateCommittedResource(&HeapDefault(device->GetNodeMask()), D3D12_HEAP_FLAG_NONE, &ResourceBuffer(totalVertexSize), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_vertexBuffer.GetAddressOf()));

        D3D12_SUBRESOURCE_DATA data;
        data.pData = vertexData;
        data.RowPitch = sizeOfVertexType;
        data.SlicePitch = data.RowPitch * vertexCount;

        shared_ptr<UploadBuffer> uploadBuffer = make_shared<UploadBuffer>(commandList, m_vertexBuffer.Get(), &data);
        commandList->GetInterface()->ResourceBarrier(1, &ResourceBarrierTransition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));

        m_vertexView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
        m_vertexView.SizeInBytes = static_cast<UINT>(totalVertexSize);
        m_vertexView.StrideInBytes = static_cast<UINT>(sizeOfVertexType);

        m_vertexCount = vertexCount;
        return uploadBuffer;
    }

    shared_ptr<UploadBuffer> Mesh::CreateIndexBuffer(const BaseCommandList* const commandList, const UINT* const indexData, const UINT& amountOfIndices)
    {
        const BaseDevice* device = commandList->GetDevice();
        device->GetInterface()->CreateCommittedResource(&HeapDefault(device->GetNodeMask()), D3D12_HEAP_FLAG_NONE, &ResourceBuffer(sizeof(UINT) * amountOfIndices), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_indexBuffer.GetAddressOf()));

        D3D12_SUBRESOURCE_DATA data;
        data.pData = indexData;
        data.RowPitch = sizeof(UINT) * 3;
        data.SlicePitch = data.RowPitch * amountOfIndices / 3;

        shared_ptr<UploadBuffer> uploadBuffer = make_shared<UploadBuffer>(commandList, m_indexBuffer.Get(), &data);
        commandList->GetInterface()->ResourceBarrier(1, &ResourceBarrierTransition(m_indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));

        m_indexView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
        m_indexView.Format = DXGI_FORMAT_R32_UINT;
        m_indexView.SizeInBytes = sizeof(UINT) * amountOfIndices;

        m_indexCount = amountOfIndices;
        return uploadBuffer;
    }

    void Mesh::SetTopology(BaseCommandList* commandList) const
    {
        commandList->GetInterface()->IASetPrimitiveTopology(m_defaultTopology);
    }

    void Mesh::SetForDraw(BaseCommandList* commandList) const
    {
        if (m_indexBuffer)
            commandList->GetInterface()->IASetIndexBuffer(&m_indexView);
        commandList->GetInterface()->IASetVertexBuffers(0, 1, &m_vertexView);
    }

    void Mesh::DrawInstance(BaseCommandList* commandList) const
    {
        commandList->GetInterface()->DrawInstanced(m_vertexCount, 1, 0, 0);
    }

    void Mesh::DrawIndexedInstance(BaseCommandList* commandList) const
    {
        commandList->GetInterface()->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
    }
}