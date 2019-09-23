#include "RendererDX12.h"
#include "Level 2/DX12CommandAllocator.h"

DX12CommandAllocator::DX12CommandAllocator() :
	m_commandAllocatorManager(nullptr)
{
}

void DX12CommandAllocator::Initialize(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type, DX12ManagerCommandAllocator* commandAllocatorManager)
{
	m_commandAllocator.Initialize(device, type);
	m_commandAllocatorManager = commandAllocatorManager;
}

D3D12_GPU_VIRTUAL_ADDRESS DX12CommandAllocator::UploadCBVSRVUAV(void* data, UINT64 size)
{
	if (m_CBVSRVUAVBuffers.empty())
		m_CBVSRVUAVBuffers.push_back(m_commandAllocatorManager->RequestCBVSRVUAVBuffer());

	if (!m_CBVSRVUAVBuffers.back()->HasSpace(size))
		m_CBVSRVUAVBuffers.push_back(m_commandAllocatorManager->RequestCBVSRVUAVBuffer());

	return m_CBVSRVUAVBuffers.back()->UploadCBVSRVUAV(data, size);
}

void DX12CommandAllocator::UploadData(ID3D12GraphicsCommandList* commandList, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data, UINT64 intermediateOffset, UINT numSubResources, UINT firstSubResource)
{
	m_temporaryBuffers.push_back(m_commandAllocatorManager->RequestTemporaryBuffer(static_cast<UINT64>(data->RowPitch) * static_cast<UINT64>(data->SlicePitch)));
	m_temporaryBuffers.back()->UploadData(commandList, destination, data, intermediateOffset, numSubResources, firstSubResource);
}

void DX12CommandAllocator::Reset()
{
	m_commandAllocator.GetInterface()->Reset();
	m_temporaryBuffers.clear();
	m_commandAllocatorManager->ResetCBVSRVUABBuffers(m_CBVSRVUAVBuffers);
}
