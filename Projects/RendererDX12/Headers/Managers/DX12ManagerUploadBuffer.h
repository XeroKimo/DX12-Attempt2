#pragma once
#include "DX12Header.h"
#include "Objects/DX12UploadBuffer.h"

class DX12ManagerUploadBuffer
{
public:
	DX12ManagerUploadBuffer();
	void Initialize(ID3D12Device* device, uint64_t standardBufferSize);

	shared_ptr<DX12UploadBuffer> GetBuffer();
	inline shared_ptr<DX12UploadBuffer> GetTemporaryBuffer(uint64_t size) { return CreateBuffer(size); }

	void ResetBuffers(std::vector<shared_ptr<DX12UploadBuffer>> buffers);

private:
	shared_ptr<DX12UploadBuffer> CreateBuffer(uint64_t size);
private:
	ID3D12Device* m_device;
	std::vector<shared_ptr<DX12UploadBuffer>> m_inactiveBuffers;
	uint64_t m_standardBufferSize;
};
