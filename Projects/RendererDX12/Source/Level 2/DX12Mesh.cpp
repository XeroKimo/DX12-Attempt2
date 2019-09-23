#include "RendererDX12.h"
#include "Level 2/DX12Mesh.h"

DX12Mesh::DX12Mesh() :
	m_indexCount(0),
	m_vertexCount(0),
	m_vertexView(),
	m_indexView()
{
}

void DX12Mesh::CreateVertexBuffer(shared_ptr<DX12CommandList>& commandList, void* vertexData, UINT sizeOfVertex, UINT vertexCount)
{
	ComPtr<ID3D12Device> device;
	HRESULT hr = commandList->GetInterface()->GetDevice(IID_PPV_ARGS(device.GetAddressOf()));
	assert(SUCCEEDED(hr));

	size_t totalVertexSize = sizeOfVertex * vertexCount;

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = device->GetNodeCount();
	heapProperties.VisibleNodeMask = heapProperties.CreationNodeMask;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Width = totalVertexSize;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Height = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_vertexBuffer.GetAddressOf()));
	assert(SUCCEEDED(hr));

	D3D12_SUBRESOURCE_DATA data;
	data.pData = vertexData;
	data.RowPitch = totalVertexSize;
	data.SlicePitch = 1;

	commandList->UploadData(m_vertexBuffer.Get(), &data, 0, 1);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = m_vertexBuffer.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	commandList->GetInterface()->ResourceBarrier(1, &barrier);

	m_vertexView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexView.SizeInBytes = totalVertexSize;
	m_vertexView.StrideInBytes = sizeOfVertex;

	m_vertexCount = vertexCount;
}

void DX12Mesh::Set(shared_ptr<DX12CommandList>& commandList)
{
	commandList->GetInterface()->IASetVertexBuffers(0, 1, &m_vertexView);
}

void DX12Mesh::Draw(shared_ptr<DX12CommandList>& commandList)
{
	commandList->GetInterface()->DrawInstanced(m_vertexCount, 1, 0, 0);
}
