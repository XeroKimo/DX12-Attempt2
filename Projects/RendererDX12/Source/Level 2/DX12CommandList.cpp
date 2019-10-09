#include "RendererDX12.h"
#include "Level 2/DX12CommandList.h"

DX12CommandList::DX12CommandList(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, unique_ptr<DX12CommandAllocator> allocator) :
    m_commandList(device,nodeMask, type, allocator->GetBase()->GetInterface()),
    m_allocator(),
    m_nodeMask(nodeMask)
{
    m_allocator.swap(allocator);
}

void DX12CommandList::Reset(unique_ptr<DX12CommandAllocator> allocator)
{
	m_commandList.GetInterface()->Reset(allocator->GetBase()->GetInterface(), nullptr);
	m_allocator.swap(allocator);
}

void DX12CommandList::SetConstantBuffer(UINT rootParamIndex, void* data, UINT64 size)
{
	m_commandList.GetInterface()->SetGraphicsRootConstantBufferView(rootParamIndex, m_allocator->UploadDynamicCBV(data, size));
}

void DX12CommandList::UploadData(ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data)
{
	m_allocator->UploadData(m_commandList.GetInterface(), m_nodeMask, destination, data);
}
