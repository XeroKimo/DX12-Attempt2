#pragma once
#include "DX12Header.h"
#include "Level 1/DX12UploadBuffer.h"

class DX12ManagerUploadBuffer
{
public:
	DX12ManagerUploadBuffer();
	void Initialize(ID3D12Device* device, UINT64 CBVSRVUAVBufferSize);
	void ResetBuffers(std::vector<shared_ptr<DX12UploadBuffer>>& buffers);

	shared_ptr<DX12UploadBuffer> GetCBVSRVUAVBuffer();
	inline shared_ptr<DX12UploadBuffer> GetTemporaryBuffer(UINT64 size) { return CreateBuffer(size); }
private:
	shared_ptr<DX12UploadBuffer> CreateBuffer(UINT64 size);
private:
	ID3D12Device* m_device;
	std::vector<shared_ptr<DX12UploadBuffer>> m_inactiveBuffers;
	UINT64 m_CBVSRVUAVBufferSize;
};
