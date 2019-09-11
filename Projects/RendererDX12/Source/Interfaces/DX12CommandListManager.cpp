#include "RendererDX12.h"
#include "Interfaces/DX12CommandListManager.h"

DX12CommandListManager::DX12CommandListManager() :
    m_allocatorManager(nullptr),
    m_device(nullptr),
    m_type(D3D12_COMMAND_LIST_TYPE_DIRECT)
{
}

void DX12CommandListManager::Initialize(DX12Device* device, DX12CommandAllocatorManager* allocatorManager, D3D12_COMMAND_LIST_TYPE type, UINT commandQueueAmount)
{
	m_device = device;
	m_allocatorManager = allocatorManager;
	m_type = type;
	m_waitingLists.resize(commandQueueAmount);
	m_waitingAllocators.resize(commandQueueAmount);
}

shared_ptr<DX12CommandList> DX12CommandListManager::GetCommandList(UINT queuePreference)
{
	shared_ptr<DX12CommandList> list;
	if (m_inactiveLists.empty())
	{
		list = make_shared<DX12CommandList>();
		list->Initialize(m_device, this, m_allocatorManager->GetAllocator(m_type), m_type, queuePreference);
	}
	else
	{
		list = m_inactiveLists.back();
		m_inactiveLists.pop_back();
		list->Reset(m_allocatorManager->GetAllocator(m_type), queuePreference);
	}
	return list;
}

shared_ptr<DX12CommandAllocator> DX12CommandListManager::RequestNewAllocator()
{
	return m_allocatorManager->GetAllocator(m_type);
}


void DX12CommandListManager::ExecuteList(UINT queueIndex)
{
	std::vector<shared_ptr<DX12CommandList>>& listToExecute = m_waitingLists[queueIndex];
	if (listToExecute.empty())
		return;

	std::vector<ID3D12CommandList*> commandLists;
	for (int v = 0; v < listToExecute.size(); v++)
	{
		commandLists.push_back(listToExecute[v]->GetCommandList());
		m_inactiveLists.push_back(listToExecute[v]);
	}

	m_device->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), commandLists.data(), queueIndex);
	m_device->GetCommandQueue(m_type, queueIndex)->SetActiveAllocator(m_waitingAllocators[queueIndex]);
	m_waitingAllocators[queueIndex].clear();
	listToExecute.clear();
}

void DX12CommandListManager::ExecuteAllLists()
{
	for (int i = 0; i < m_waitingLists.size(); i++)
	{
		ExecuteList(i);
	}
}

void DX12CommandListManager::ReEnlistCommandList(shared_ptr<DX12CommandList> commandList)
{
	for (int i = 0; i < m_inactiveLists.size(); i++)
	{
		if (m_inactiveLists[i] == commandList)
			return;
	}
	m_inactiveLists.push_back(commandList);
}

void DX12CommandListManager::CloseList(shared_ptr<DX12CommandList> list)
{
	m_waitingLists[list->GetQueuePreference()].push_back(list);
	m_waitingAllocators[list->GetQueuePreference()].push_back(list->GetCommandAllocator());
}
