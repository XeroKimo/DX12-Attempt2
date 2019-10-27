#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class UploadBuffer;
    class Mesh
    {
    public:
        Mesh();

        shared_ptr<UploadBuffer> CreateVertexBuffer(const BaseCommandList* const commandList, const void* const vertexData, const UINT& sizeOfVerteType, const UINT& vertexCount);
        shared_ptr<UploadBuffer> CreateIndexBuffer(const BaseCommandList* const commandList, const UINT* const indexData, const UINT& amountOfIndices);

        void ChangeDefaultTopology(D3D_PRIMITIVE_TOPOLOGY topology) { m_defaultTopology = topology; }
        void SetTopology(BaseCommandList* commandList) const;
        void SetForDraw(BaseCommandList* commandList) const;
        void DrawInstance(BaseCommandList* commandList) const;
        void DrawIndexedInstance(BaseCommandList* commandList) const;

        ID3D12Resource* GetVertexBuffer() const noexcept { return m_vertexBuffer.Get(); }
        ID3D12Resource* GetIndexBuffer() const noexcept { return m_indexBuffer.Get(); }

        D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const noexcept { return m_vertexView; }
        D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const noexcept { return m_indexView; }

        UINT GetVertexCount() const noexcept { return m_vertexCount; }
        UINT GetIndexCount() const noexcept { return m_indexCount; }
    private:
        ComPtr<ID3D12Resource> m_vertexBuffer;
        ComPtr<ID3D12Resource> m_indexBuffer;

        D3D12_VERTEX_BUFFER_VIEW m_vertexView;
        D3D12_INDEX_BUFFER_VIEW m_indexView;

        D3D_PRIMITIVE_TOPOLOGY m_defaultTopology;

        UINT m_vertexCount;
        UINT m_indexCount;
    };
}