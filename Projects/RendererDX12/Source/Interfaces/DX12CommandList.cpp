#include "RendererDX12.h"

DX12CommandList::DX12CommandList()
{
}

void DX12CommandList::Initialize(DX12Device* device, DX12CommandListManager* manager, shared_ptr<DX12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type, UINT queuePreference)
{
	if (FAILED(device->GetDevice()->CreateCommandList(device->GetNodeMask(), type, allocator->GetAllocator().Get(), nullptr, IID_PPV_ARGS(m_commandList.GetAddressOf()))))
		assert(false);
	m_allocator = allocator;
	m_manager = manager;
	m_device = device;
	m_queuePreference = queuePreference;
}

void DX12CommandList::Reset(shared_ptr<DX12CommandAllocator> allocator, UINT queuePreference)
{
	m_commandList->Reset(allocator->GetAllocator().Get(), nullptr);
	m_allocator = allocator;
	m_queuePreference = queuePreference;
}

void DX12CommandList::LocalReset(UINT queuePreference)
{
	Reset(m_manager->RequestNewAllocator(), queuePreference);
}

void DX12CommandList::ReEnlistCommandList(shared_ptr<DX12CommandList>& commandList)
{
	commandList->LocalReEnlist();
	commandList = nullptr;
}

void DX12CommandList::LocalReEnlist()
{
	m_manager->ReEnlistCommandList(shared_from_this());
}

void DX12CommandList::Close()
{
	if (FAILED(m_commandList->Close()))
		return;
	m_manager->CloseList(shared_from_this());
	m_device->GetCommandQueue(m_commandList->GetType(), m_queuePreference)->SetActiveAllocator(m_allocator);
	m_allocator = nullptr;
}

void DX12CommandList::ExecuteAll()
{
	m_manager->ExecuteList();
}

void DX12CommandList::CloseAndExecuteThis()
{
	if (FAILED(m_commandList->Close()))
		return;
	ID3D12CommandList* list[] = { m_commandList.Get() };
	m_device->ExecuteCommandLists(1, list, m_queuePreference);
	m_device->GetCommandQueue(m_commandList->GetType(), m_queuePreference)->SetActiveAllocator(m_allocator);
	m_allocator = nullptr;
}


