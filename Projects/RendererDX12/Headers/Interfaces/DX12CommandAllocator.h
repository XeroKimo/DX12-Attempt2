#pragma once
#include "DX12Header.h"
#include "BaseObjects/DX12BaseCommandAllocator.h"
class DX12UploadBuffer;
class DX12ManagerUploadBuffer;

class DX12CommandAllocator
{
public:
	DX12CommandAllocator();
	void Initialize(ID3D12Device* device, DX12ManagerUploadBuffer* uploadBufferManger, const D3D12_COMMAND_LIST_TYPE& type);

	D3D12_GPU_VIRTUAL_ADDRESS UploadCBVSRVUAV(void* data, uint64_t size);
	void UploadData(ID3D12GraphicsCommandList* commandList, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data, UINT64 intermediateOffset, UINT numSubResources, UINT firstSubResource = 0);
	void Reset();

	inline DX12BaseCommandAllocator* GetBaseAllocator() { return &m_commandAllocator; }
private:
	DX12ManagerUploadBuffer* m_uploadBufferManger;
	DX12BaseCommandAllocator m_commandAllocator;
	std::vector<shared_ptr<DX12UploadBuffer>> m_CBVSRVUAVBuffers;
	std::vector<shared_ptr<DX12UploadBuffer>> m_temporaryBuffers;
};