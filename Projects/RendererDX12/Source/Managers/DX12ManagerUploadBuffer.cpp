#include "RendererDX12.h"
#include "Managers/DX12ManagerUploadBuffer.h"

DX12ManagerUploadBuffer::DX12ManagerUploadBuffer()
{
}

void DX12ManagerUploadBuffer::Initialize(ID3D12Device* device, uint64_t standardBufferSize)
{
	int power = static_cast<int>(pow(2, 16)) - 1;
	m_device = device;
	m_standardBufferSize = (standardBufferSize + power) & ~power;
}

void DX12ManagerUploadBuffer::ResetBuffers(std::vector<shared_ptr<DX12UploadBuffer>>  buffers)
{
	std::copy(buffers.begin(), buffers.end(), std::back_inserter(m_inactiveBuffers));
	for (shared_ptr<DX12UploadBuffer>& buffer : m_inactiveBuffers)
		buffer->Reset();
}

shared_ptr<DX12UploadBuffer> DX12ManagerUploadBuffer::CreateBuffer(uint64_t size)
{
	shared_ptr<DX12UploadBuffer> buffer = make_shared<DX12UploadBuffer>();
	buffer->Initialize(m_device, size);
	return buffer;
}

shared_ptr<DX12UploadBuffer> DX12ManagerUploadBuffer::GetBuffer()
{
	if (m_inactiveBuffers.empty())
		return CreateBuffer(m_standardBufferSize);
	shared_ptr<DX12UploadBuffer> buffer = m_inactiveBuffers.back();
	m_inactiveBuffers.pop_back();
	return buffer;
}
