#include "RendererDX12.h"
#include "Level 1/DX12UploadBuffer.h"

DX12UploadBuffer::DX12UploadBuffer() :
	m_totalSize(0),
	m_usedSize(0),
    m_mappedAddress(nullptr)
{
}

void DX12UploadBuffer::Initialize(ID3D12Device* device, UINT creationNodeMask, UINT visibleNodeMask, UINT64 size)
{
	UINT64 alignedSize = (size + ((1 << 16) - 1)) & ~((1 << 16) - 1);

	D3D12_HEAP_PROPERTIES heapProperties;
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = creationNodeMask;
	heapProperties.VisibleNodeMask = visibleNodeMask;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Width = size;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Height = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Alignment = 0;

	HRESULT hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
	assert(SUCCEEDED (hr));

	D3D12_RANGE range = { 0,0 };
	m_resource->Map(0, &range, reinterpret_cast<void**>(&m_mappedAddress));

	m_totalSize = size;
}

D3D12_GPU_VIRTUAL_ADDRESS DX12UploadBuffer::UploadCBVSRVUAV(void* data, UINT64 size)
{
	D3D12_GPU_VIRTUAL_ADDRESS address = m_resource->GetGPUVirtualAddress() + m_usedSize;
	memcpy(m_mappedAddress + m_usedSize, data, static_cast<size_t>(size));
	m_usedSize += (static_cast<size_t>(size) + ConstBufferSize - 1) & ~(ConstBufferSize - 1);
	return address;
}

void DX12UploadBuffer::UploadData(ID3D12GraphicsCommandList* commandList, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data, UINT64 intermediateOffset, UINT numSubResources, UINT firstSubResource)
{
	m_resource->Unmap(0, nullptr);
	UpdateSubresources(commandList, destination, m_resource.Get(), intermediateOffset, firstSubResource, numSubResources, data);
}

void DX12UploadBuffer::Reset()
{
    ZeroMemory(m_mappedAddress, static_cast<size_t>(m_totalSize));
	m_usedSize = 0;
}
