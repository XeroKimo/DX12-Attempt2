#include "RendererDX12.h"

DX12CommandList::DX12CommandList() :
	m_manager(nullptr),
	m_queuePreference(0)
{
}

void DX12CommandList::Initialize(ID3D12Device* device, DX12ManagerCommandList* manager, shared_ptr<DX12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type, UINT queuePreference)
{
	HRESULT hr = device->CreateCommandList(device->GetNodeCount(), type, allocator->GetBaseAllocator()->GetAllocator().Get(), nullptr, IID_PPV_ARGS(m_commandList.GetAddressOf()));

	assert(SUCCEEDED(hr));
	m_allocator = allocator;
	m_manager = manager;
	m_queuePreference = queuePreference;
}

void DX12CommandList::Reset(shared_ptr<DX12CommandAllocator> allocator, UINT queuePreference)
{
	m_commandList->Reset(allocator->GetBaseAllocator()->GetAllocator().Get(), nullptr);
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
    commandList->m_commandList->Close();
    commandList->m_manager->ExecuteDirect(commandList);
    commandList->m_manager->ReEnlistCommandList(commandList);
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
    commandList->m_commandList->Close();
    commandList->m_manager->ExecuteDirect(commandList);
	commandList->Reset(commandList->m_manager->RequestNewAllocator(), queuePreference);
}

void DX12CommandList::CloseList(shared_ptr<DX12CommandList>& commandList)
{
	commandList->m_commandList->Close();
	commandList->m_manager->CloseList(commandList);
}

