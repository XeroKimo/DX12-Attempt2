#include "RendererDX12.h"
#include "Level 2/DX12CommandAllocator.h"

DX12CommandAllocator::DX12CommandAllocator() :
	m_bufferManager(nullptr)
{
}

DX12CommandAllocator::~DX12CommandAllocator()
{
}

void DX12CommandAllocator::Initialize(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type, DX12ManagerConstBuffer* bufferManager)
{
	m_commandAllocator.Initialize(device, type);
	m_bufferManager = bufferManager;
}

D3D12_GPU_VIRTUAL_ADDRESS DX12CommandAllocator::UploadDynamicCBV(void* data, UINT64 size)
{
	if (m_constBuffers.empty())
		m_constBuffers.push_back(m_bufferManager->GetConstBuffer());

	if (!m_constBuffers.back()->HasSpace(size))
		m_constBuffers.push_back(m_bufferManager->GetConstBuffer());

	return m_constBuffers.back()->UploadCBVSRVUAV(data, size);
}

void DX12CommandAllocator::UploadData(ID3D12GraphicsCommandList* commandList, UINT nodeMask, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data)
{
	m_temporaryBuffers.push_back(make_shared<DX12UploadBuffer>(commandList, nodeMask, destination, data));
}

void DX12CommandAllocator::Reset()
{
	m_commandAllocator.GetInterface()->Reset();
	m_bufferManager->ResetBuffers(m_constBuffers);
	m_constBuffers.clear();
	m_temporaryBuffers.clear();
}