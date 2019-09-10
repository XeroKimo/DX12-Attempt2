#include "RendererDX12.h"

DX12CommandList::DX12CommandList()
{
}

void DX12CommandList::Initialize(DX12Device* device, DX12CommandListManager* manager, shared_ptr<DX12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type)
{
	if (FAILED(device->GetDevice()->CreateCommandList(device->GetNodeMask(), type, allocator->GetAllocator().Get(), nullptr, IID_PPV_ARGS(m_commandList.GetAddressOf()))))
		assert(false);
	m_allocator = allocator;
	m_manager = manager;
	
}

void DX12CommandList::Reset(shared_ptr<DX12CommandAllocator> allocator)
{
	m_commandList->Reset(allocator->GetAllocator().Get(), nullptr);
	m_allocator = allocator;
}

void DX12CommandList::Close()
{
	m_commandList->Close();
	m_manager->CloseList(shared_from_this());
	m_allocator->SetActive();
}

void DX12CommandList::Execute()
{
	m_manager->ExecuteList();
}


