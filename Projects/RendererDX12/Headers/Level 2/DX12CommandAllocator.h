#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseCommandAllocator.h"
#include "Level 1/DX12UploadBuffer.h"
class DX12ManagerUploadBuffer;

class DX12CommandAllocator
{
public:
	DX12CommandAllocator();
	~DX12CommandAllocator();
	void Initialize(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type, DX12ManagerUploadBuffer* m_bufferManager);

	D3D12_GPU_VIRTUAL_ADDRESS UploadCBVSRVUAV(void* data, UINT64 size);
	void UploadData(ID3D12GraphicsCommandList* commandList, UINT nodeMask, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data);
	void Reset();

	inline DX12BaseCommandAllocator* GetBase() { return &m_commandAllocator; }
private:
	DX12ManagerUploadBuffer* m_bufferManager;
	DX12BaseCommandAllocator m_commandAllocator;
	std::vector<unique_ptr<DX12UploadBuffer>> m_CBVSRVUAVBuffers;
	std::vector<unique_ptr<DX12UploadBuffer>> m_temporaryBuffers;
};