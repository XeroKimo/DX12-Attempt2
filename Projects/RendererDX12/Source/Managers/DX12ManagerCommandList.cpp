#include "RendererDX12.h"
#include "Managers/DX12ManagerCommandList.h"

DX12ManagerCommandList::DX12ManagerCommandList() :
    m_allocatorManager(nullptr),
    m_device(nullptr),
    m_type(D3D12_COMMAND_LIST_TYPE_DIRECT)
{
}

void DX12ManagerCommandList::Initialize(ID3D12Device* device, std::vector<unique_ptr<DX12CommandQueue>>* commandQueues, DX12ManagerCommandAllocator* allocatorManager, D3D12_COMMAND_LIST_TYPE type, UINT commandQueueAmount)
{
    m_device = device;
	m_pCommandQueues = commandQueues;
	m_allocatorManager = allocatorManager;
	m_type = type;
	m_waitingLists.resize(commandQueueAmount);
	m_waitingAllocators.resize(commandQueueAmount);
}

shared_ptr<DX12CommandList> DX12ManagerCommandList::GetCommandList(UINT queuePreference)
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

shared_ptr<DX12CommandAllocator> DX12ManagerCommandList::RequestNewAllocator()
{
	return m_allocatorManager->GetAllocator(m_type);
}


void DX12ManagerCommandList::ExecuteList(UINT queueIndex)
{
	std::vector<shared_ptr<DX12CommandList>>& listToExecute = m_waitingLists[queueIndex];
	if (listToExecute.empty())
		return;

	std::vector<ID3D12CommandList*> commandLists;
	for (size_t v = 0; v < listToExecute.size(); v++)
	{
		commandLists.push_back(listToExecute[v]->GetCommandList());
		m_inactiveLists.push_back(listToExecute[v]);
	}

    (*m_pCommandQueues)[queueIndex]->GetBaseCommandQueue()->GetCommandQueue()->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), commandLists.data());
    (*m_pCommandQueues)[queueIndex]->SetActiveAllocators(m_waitingAllocators[queueIndex]);
	m_waitingAllocators[queueIndex].clear();
	listToExecute.clear();
}

void DX12ManagerCommandList::ExecuteAllLists()
{
	for (size_t i = 0; i < m_waitingLists.size(); i++)
	{
		ExecuteList(i);
	}
}

void DX12ManagerCommandList::ExecuteDirect(shared_ptr<DX12CommandList> commandList)
{
    ID3D12CommandList* list[] = { commandList->GetCommandList() };
    (*m_pCommandQueues)[commandList->GetQueuePreference()]->GetBaseCommandQueue()->GetCommandQueue()->ExecuteCommandLists(1, list);
    (*m_pCommandQueues)[commandList->GetQueuePreference()]->SetActiveAllocator(commandList->GetCommandAllocator());
}

void DX12ManagerCommandList::ReEnlistCommandList(shared_ptr<DX12CommandList> commandList)
{
	m_inactiveLists.push_back(commandList);
}

void DX12ManagerCommandList::CloseList(shared_ptr<DX12CommandList> list)
{
	m_waitingLists[list->GetQueuePreference()].push_back(list);
	m_waitingAllocators[list->GetQueuePreference()].push_back(list->GetCommandAllocator());
}
