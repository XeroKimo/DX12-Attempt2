#pragma once
#include "DX12Header.h"
#include "Level 2/DX12CommandAllocator.h"
#include "Level 2/DX12ManagerUploadBuffer.h"

class DX12ManagerCommandAllocator
{
public:
	DX12ManagerCommandAllocator();

	void Initialize(ID3D12Device* device, DX12ManagerUploadBuffer* bufferManager);
	void ResetAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocators);

	unique_ptr<DX12CommandAllocator> GetAllocator(const D3D12_COMMAND_LIST_TYPE& type);
	inline shared_ptr<DX12UploadBuffer> RequestCBVSRVUAVBuffer() { return m_bufferManager->GetCBVSRVUAVBuffer(); }
private:
	unique_ptr<DX12CommandAllocator> CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
private:
    ID3D12Device* m_device;
	DX12ManagerUploadBuffer* m_bufferManager;
	std::vector<unique_ptr<DX12CommandAllocator>> m_directAllocators;
	//std::vector<unique_ptr<DX12CommandAllocator>> m_bundleAllocators;
	std::vector<unique_ptr<DX12CommandAllocator>> m_copyAllocators;
	std::vector<unique_ptr<DX12CommandAllocator>> m_computeAllocators;
};