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

void DX12CommandList::Close(shared_ptr<DX12CommandList>& commandList)
{
	CloseList(commandList);
	commandList = nullptr;
}

void DX12CommandList::CloseAndExecute(shared_ptr<DX12CommandList>& commandList)
{
	Execute(commandList);
	commandList = nullptr;
}

void DX12CommandList::CloseAndExecuteAll(shared_ptr<DX12CommandList>& commandList)
{
	CloseList(commandList);
	commandList->m_manager->ExecuteList(commandList->m_queuePreference);
	commandList = nullptr;
}

void DX12CommandList::CloseExecuteReset(shared_ptr<DX12CommandList>& commandList, UINT queuePreference)
{
	Execute(commandList);
	commandList->Reset(commandList->m_manager->RequestNewAllocator(), queuePreference);
}

void DX12CommandList::CloseList(shared_ptr<DX12CommandList> commandList)
{
	commandList->m_commandList->Close();
	commandList->m_manager->CloseList(commandList);
	commandList->m_allocator = nullptr;
}

void DX12CommandList::Execute(shared_ptr<DX12CommandList> commandList)
{
	commandList->m_commandList->Close();
	commandList->m_manager->ReEnlistCommandList(commandList);
	ID3D12CommandList* list[] = { commandList->m_commandList.Get() };
	commandList->m_device->ExecuteCommandLists(1, list, commandList->m_queuePreference);
	commandList->m_device->GetCommandQueue(commandList->m_commandList->GetType(), commandList->m_queuePreference)->SetActiveAllocator(commandList->m_allocator);
	commandList->m_allocator = nullptr;
}

