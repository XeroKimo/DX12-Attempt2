#include "RendererDX12.h"
#include "Level 2/DX12ManagerUploadBuffer.h"

DX12ManagerUploadBuffer::DX12ManagerUploadBuffer() :
	m_device(nullptr),
	m_CBVSRVUAVBufferSize(1<<16)
{
}

void DX12ManagerUploadBuffer::Initialize(ID3D12Device* device, UINT creationNodeMask, UINT visibleNodeMask, UINT64 CBVSRVUAVBufferSize)
{
	int power = (1 << 16) - 1;
	m_device = device;
	m_CBVSRVUAVBufferSize = (CBVSRVUAVBufferSize + power) & ~power;
	m_creationNodeMask = creationNodeMask;
	m_visibleNodeMask = visibleNodeMask;
}

void DX12ManagerUploadBuffer::ResetBuffers(std::vector<shared_ptr<DX12UploadBuffer>> buffers)
{
	std::move(buffers.begin(), buffers.end(), std::back_inserter(m_inactiveBuffers));
}

shared_ptr<DX12UploadBuffer> DX12ManagerUploadBuffer::GetCBVSRVUAVBuffer()
{
	if (m_inactiveBuffers.empty())
		return CreateBuffer(m_CBVSRVUAVBufferSize);
	shared_ptr<DX12UploadBuffer> buffer = m_inactiveBuffers.back();
	m_inactiveBuffers.pop_back();
	buffer->Reset();
	return buffer;
}

shared_ptr<DX12UploadBuffer> DX12ManagerUploadBuffer::CreateBuffer(UINT64 size)
{
	shared_ptr<DX12UploadBuffer> buffer = make_shared<DX12UploadBuffer>();
	buffer->Initialize(m_device, m_creationNodeMask, m_visibleNodeMask, size);
	return buffer;
}
