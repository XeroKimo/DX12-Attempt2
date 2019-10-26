#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class UploadBuffer;
    class Mesh
    {
    public:
        Mesh();

        shared_ptr<UploadBuffer> CreateVertexBuffer(BaseCommandList* commandList, void* vertexData, UINT sizeOfVerteType, UINT vertexCount);
        shared_ptr<UploadBuffer> CreateIndexBuffer(BaseCommandList* commandList, UINT* indexData, UINT amountOfIndices);

        void ChangeDefaultTopology(D3D_PRIMITIVE_TOPOLOGY topology) { m_defaultTopology = topology; }
        void SetTopology(BaseCommandList* commandList);
        void SetForDraw(BaseCommandList* commandList);
        void DrawInstance(BaseCommandList* commandList);
        void DrawIndexedInstance(BaseCommandList* commandList);

        ID3D12Resource* GetVertexBuffer() { return m_vertexBuffer.Get(); }
        ID3D12Resource* GetIndexBuffer() { return m_indexBuffer.Get(); }

        D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() { return m_vertexView; }
        D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() { return m_indexView; }

        UINT GetVertexCount() { return m_vertexCount; }
        UINT GetIndexCount() { return m_indexCount; }
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