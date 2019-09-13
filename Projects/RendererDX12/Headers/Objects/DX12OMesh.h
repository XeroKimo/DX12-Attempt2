#pragma once
#include "DX12Header.h"

class DX12OMesh
{
public:
	DX12OMesh();

	void CreateVertexBuffer(DX12CommandList* commandList, void* vertexData, UINT sizeOfVertex, UINT vertexCount, ID3D12Resource** uploadBuffer);

	void Set(shared_ptr<DX12CommandList>& commandList);
	void Draw(shared_ptr<DX12CommandList>& commandList);
private:
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_indexBuffer;

	D3D12_VERTEX_BUFFER_VIEW m_vertexView;
	D3D12_INDEX_BUFFER_VIEW m_indexView;

	UINT m_vertexCount;
	UINT m_indexCount;
};