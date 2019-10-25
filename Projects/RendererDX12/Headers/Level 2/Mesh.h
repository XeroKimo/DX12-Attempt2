#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class Mesh
    {
    public:
        Mesh();

        shared_ptr<UploadBuffer> CreateVertexBuffer(BaseCommandList* commandList, void* vertexData, UINT sizeOfVertex, UINT vertexCount);

        void Set(BaseCommandList* commandList);
        void Draw(BaseCommandList* commandList);

        ID3D12Resource* GetVertexBuffer() { return m_vertexBuffer.Get(); }
        ID3D12Resource* GetIndexBuffer() { return m_indexBuffer.Get(); }

        D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() { return m_vertexView; }
        D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() { return m_indexView; }
    private:
        ComPtr<ID3D12Resource> m_vertexBuffer;
        ComPtr<ID3D12Resource> m_indexBuffer;

        D3D12_VERTEX_BUFFER_VIEW m_vertexView;
        D3D12_INDEX_BUFFER_VIEW m_indexView;

        UINT m_vertexCount;
        UINT m_indexCount;
    };
}