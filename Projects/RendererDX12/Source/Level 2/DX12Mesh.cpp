#include "RendererDX12.h"
#include "Level 2/DX12Mesh.h"

DX12Mesh::DX12Mesh() :
	m_indexCount(0),
	m_vertexCount(0),
	m_vertexView(),
	m_indexView()
{
}

void DX12Mesh::CreateVertexBuffer(DX12CommandList* commandList, void* vertexData, UINT sizeOfVertex, UINT vertexCount)
{
	ComPtr<ID3D12Device> device;
	HRESULT hr = commandList->GetBase()->GetInterface()->GetDevice(IID_PPV_ARGS(device.GetAddressOf()));
	assert(SUCCEEDED(hr));

	LONG_PTR totalVertexSize = static_cast<LONG_PTR>(sizeOfVertex) * static_cast<LONG_PTR>(vertexCount);

    m_vertexBuffer = make_unique<DX12Resource>();
    m_vertexBuffer->InitAsBuffer(device.Get(), totalVertexSize, D3D12_RESOURCE_STATE_COPY_DEST);

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	UINT numRows;
	UINT64 sizeInBytes;
	UINT64 totalBytes;

	device->GetCopyableFootprints(&m_vertexBuffer->GetInterface()->GetDesc(), 0, 1, 0, &footprint, &numRows, &sizeInBytes, &totalBytes);

	D3D12_SUBRESOURCE_DATA data;
	data.pData = vertexData;
	data.RowPitch = sizeOfVertex;
	data.SlicePitch = data.RowPitch * vertexCount;

	commandList->UploadData(m_vertexBuffer->GetInterface(), data.SlicePitch, &data, 0, 1);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = m_vertexBuffer->GetInterface();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	commandList->GetBase()->GetInterface()->ResourceBarrier(1, &barrier);

	m_vertexView.BufferLocation = m_vertexBuffer->GetInterface()->GetGPUVirtualAddress();
	m_vertexView.SizeInBytes = totalVertexSize;
	m_vertexView.StrideInBytes = sizeOfVertex;

	m_vertexCount = vertexCount;
}

void DX12Mesh::Set(DX12CommandList* commandList)
{
	commandList->GetBase()->GetInterface()->IASetVertexBuffers(0, 1, &m_vertexView);
}

void DX12Mesh::Draw(DX12CommandList* commandList)
{
	commandList->GetBase()->GetInterface()->DrawInstanced(m_vertexCount, 1, 0, 0);
}
