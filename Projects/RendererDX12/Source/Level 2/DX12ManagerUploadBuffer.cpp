#include "RendererDX12.h"
#include "Level 2/DX12ManagerUploadBuffer.h"

DX12ManagerUploadBuffer::DX12ManagerUploadBuffer() :
	m_device(nullptr),
	m_CBVSRVUAVBufferSize(1<<16)
{
}

void DX12ManagerUploadBuffer::Initialize(DX12BaseDevice* device, UINT64 CBVSRVUAVBufferSize)
{
	int power = (1 << 16) - 1;
	m_device = device;
	m_CBVSRVUAVBufferSize = (CBVSRVUAVBufferSize + power) & ~power;
}

void DX12ManagerUploadBuffer::ResetBuffers(std::vector<unique_ptr<DX12UploadBuffer>>& buffers)
{
	std::move(buffers.begin(), buffers.end(), std::back_inserter(m_inactiveBuffers));
}

unique_ptr<DX12UploadBuffer> DX12ManagerUploadBuffer::GetCBVSRVUAVBuffer()
{
	if (m_inactiveBuffers.empty())
		return CreateBuffer(m_CBVSRVUAVBufferSize);
	unique_ptr<DX12UploadBuffer> buffer = std::move(m_inactiveBuffers.back());
	m_inactiveBuffers.pop_back();
	buffer->Reset();
	return buffer;
}

unique_ptr<DX12UploadBuffer> DX12ManagerUploadBuffer::CreateBuffer(UINT64 size)
{
	unique_ptr<DX12UploadBuffer> buffer = make_unique<DX12UploadBuffer>();
	buffer->Initialize(m_device->GetInterface(), m_device->GetNodeMask(), size);
	return buffer;
}
