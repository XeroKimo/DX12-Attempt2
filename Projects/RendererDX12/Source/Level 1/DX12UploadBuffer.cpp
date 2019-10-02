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
    m_resource.InitAsUpload(device, size, creationNodeMask, visibleNodeMask);
	D3D12_RANGE range = { 0,0 };
	m_resource.GetInterface()->Map(0, &range, reinterpret_cast<void**>(&m_mappedAddress));

	m_totalSize = size;
}

D3D12_GPU_VIRTUAL_ADDRESS DX12UploadBuffer::UploadCBVSRVUAV(void* data, UINT64 size)
{
	D3D12_GPU_VIRTUAL_ADDRESS address = m_resource.GetInterface()->GetGPUVirtualAddress() + m_usedSize;
	memcpy(m_mappedAddress + m_usedSize, data, static_cast<size_t>(size));
	m_usedSize += (static_cast<size_t>(size) + ConstBufferSize - 1) & ~(ConstBufferSize - 1);
	return address;
}

void DX12UploadBuffer::UploadData(ID3D12GraphicsCommandList* commandList, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data, UINT64 intermediateOffset, UINT numSubResources, UINT firstSubResource)
{
	m_resource.GetInterface()->Unmap(0, nullptr);
	UpdateSubresources(commandList, destination, m_resource.GetInterface(), intermediateOffset, firstSubResource, numSubResources, data);
}

void DX12UploadBuffer::Reset()
{
    ZeroMemory(m_mappedAddress, static_cast<size_t>(m_totalSize));
	m_usedSize = 0;
}
