#include "RendererDX12.h"
#include "Level 2/DX12ManagerConstBuffer.h"

DX12ManagerConstBuffer::DX12ManagerConstBuffer() :
	m_device(nullptr),
	m_bufferSize(1<<16)
{
}

void DX12ManagerConstBuffer::Initialize(DX12BaseDevice* device, UINT64 constBufferSize)
{
	int power = (1 << 16) - 1;
	m_device = device;
	m_bufferSize = (constBufferSize + power) & ~power;
}

void DX12ManagerConstBuffer::ResetBuffers(std::vector<unique_ptr<DX12UploadBuffer>>& buffers)
{
	std::move(buffers.begin(), buffers.end(), std::back_inserter(m_inactiveBuffers));
}

unique_ptr<DX12UploadBuffer> DX12ManagerConstBuffer::GetConstBuffer()
{
	if (m_inactiveBuffers.empty())
		return CreateBuffer(m_bufferSize);
	unique_ptr<DX12UploadBuffer> buffer = std::move(m_inactiveBuffers.back());
	m_inactiveBuffers.pop_back();
	buffer->Reset();
	return buffer;
}

unique_ptr<DX12UploadBuffer> DX12ManagerConstBuffer::CreateBuffer(UINT64 size)
{
	unique_ptr<DX12UploadBuffer> buffer = make_unique<DX12UploadBuffer>();
	buffer->Initialize(m_device->GetInterface(), m_device->GetNodeMask(), size);
	return buffer;
}
