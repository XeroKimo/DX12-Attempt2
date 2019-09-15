#include "RendererDX12.h"
#include "Interfaces/DX12CommandAllocator.h"

DX12CommandAllocator::DX12CommandAllocator()
{
}

void DX12CommandAllocator::Initialize(ID3D12Device* device, DX12ManagerUploadBuffer* uploadBufferManger, const D3D12_COMMAND_LIST_TYPE& type)
{
	m_commandAllocator.Initialize(device, type);
	m_uploadBufferManger = uploadBufferManger;
}

D3D12_GPU_VIRTUAL_ADDRESS DX12CommandAllocator::UploadCBVSRVUAV(void* data, uint64_t size)
{
	if (m_CBVSRVUAVBuffers.empty())
		m_CBVSRVUAVBuffers.push_back(m_uploadBufferManger->GetBuffer());

	if (!m_CBVSRVUAVBuffers.back()->HasSpace(size))
		m_CBVSRVUAVBuffers.push_back(m_uploadBufferManger->GetBuffer());

	return m_CBVSRVUAVBuffers.back()->UploadCBVSRVUAV(data, size);
}

void DX12CommandAllocator::UploadData(ID3D12GraphicsCommandList* commandList, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data, UINT64 intermediateOffset, UINT numSubResources, UINT firstSubResource)
{
	m_temporaryBuffers.push_back(m_uploadBufferManger->GetTemporaryBuffer(data->RowPitch * data->SlicePitch));
	m_temporaryBuffers.back()->UploadData(commandList, destination, data, intermediateOffset, numSubResources, firstSubResource);
}

void DX12CommandAllocator::Reset()
{
	m_commandAllocator.GetAllocator()->Reset();
	m_temporaryBuffers.clear();
	m_uploadBufferManger->ResetBuffers(m_CBVSRVUAVBuffers);
}
