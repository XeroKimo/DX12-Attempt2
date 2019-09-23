#pragma once
#include "DX12Header.h"
#include "DX12CommandAllocator.h"
#include "Level 1/DX12ManagerUploadBuffer.h"

class DX12ManagerCommandAllocator
{
public:
	DX12ManagerCommandAllocator();

	void Initialize(ID3D12Device* device, DX12ManagerUploadBuffer* bufferManager);
	void ResetAllocators(std::vector<shared_ptr<DX12CommandAllocator>> allocators);

	shared_ptr<DX12CommandAllocator> GetAllocator(const D3D12_COMMAND_LIST_TYPE& type);
	inline shared_ptr<DX12UploadBuffer> RequestCBVSRVUAVBuffer() { return m_bufferManager->GetCBVSRVUAVBuffer(); }
	inline shared_ptr<DX12UploadBuffer> RequestTemporaryBuffer(UINT64 size) { return m_bufferManager->GetTemporaryBuffer(size); }
	inline void ResetCBVSRVUABBuffers(std::vector<shared_ptr<DX12UploadBuffer>> buffers) { m_bufferManager->ResetBuffers(buffers); }
private:
	shared_ptr<DX12CommandAllocator> CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
private:
    ID3D12Device* m_device;
	DX12ManagerUploadBuffer* m_bufferManager;
	std::vector<shared_ptr<DX12CommandAllocator>> m_directAllocators;
	//std::vector<shared_ptr<DX12CommandAllocator>> m_bundleAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_copyAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_computeAllocators;
};