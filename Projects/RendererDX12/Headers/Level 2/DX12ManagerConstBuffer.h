#pragma once
#include "DX12Header.h"
#include "Level 1/DX12UploadBuffer.h"

class DX12ManagerConstBuffer
{
public:
	DX12ManagerConstBuffer(DX12BaseDevice* device, UINT64 constBufferSize);
	void ResetBuffers(std::vector<unique_ptr<DX12UploadBuffer>>& buffers);

	unique_ptr<DX12UploadBuffer> GetConstBuffer();
private:
	unique_ptr<DX12UploadBuffer> CreateBuffer(UINT64 size);
private:
	DX12BaseDevice* m_device;
	std::vector<unique_ptr<DX12UploadBuffer>> m_inactiveBuffers;
	UINT64 m_bufferSize;
};
