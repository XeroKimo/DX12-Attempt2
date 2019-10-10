#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class Mesh
    {
    public:
        Mesh();

        void CreateVertexBuffer(CommandList* commandList, void* vertexData, UINT sizeOfVertex, UINT vertexCount);

        void Set(CommandList* commandList);
        void Draw(CommandList* commandList);
    private:
        ComPtr<ID3D12Resource> m_vertexBuffer;
        ComPtr<ID3D12Resource> m_indexBuffer;

        D3D12_VERTEX_BUFFER_VIEW m_vertexView;
        D3D12_INDEX_BUFFER_VIEW m_indexView;

        UINT m_vertexCount;
        UINT m_indexCount;
    };
}