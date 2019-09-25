#include "RendererDX12.h"
#include "Level 3/DX12ManagerCommandList.h"

DX12ManagerCommandList::DX12ManagerCommandList() :
    m_allocatorManager(nullptr),
    m_device(nullptr),
    m_type(D3D12_COMMAND_LIST_TYPE_DIRECT),
    m_pCommandQueues()
{
}

void DX12ManagerCommandList::Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, std::vector<unique_ptr<DX12CommandQueue>>* commandQueues, DX12ManagerCommandAllocator* allocatorManager)
{
    assert(type != D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE);
    assert(type != D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS);
    assert(type != D3D12_COMMAND_LIST_TYPE_BUNDLE);

    m_device = device;
	m_pCommandQueues = commandQueues;
	m_allocatorManager = allocatorManager;
	m_type = type;
	m_waitingLists.resize(commandQueues->size());
	m_nodeMask = nodeMask;
}

void DX12ManagerCommandList::CloseList(shared_ptr<DX12CommandList>& list, UINT queueIndex)
{
	assert(queueIndex < m_pCommandQueues->size());

	list->Close();
	m_waitingLists[queueIndex].AddCommandList(list);
	list = nullptr;
}

void DX12ManagerCommandList::ExecuteList(shared_ptr<DX12CommandList>& commandList, UINT queueIndex)
{
	assert(queueIndex < m_pCommandQueues->size());

	commandList->Close();
	ID3D12CommandList* list[1] = { commandList->GetBase()->GetInterface() };
	(*m_pCommandQueues)[queueIndex]->ExecuteCommandLists(1, list);
	(*m_pCommandQueues)[queueIndex]->SetActiveAllocator(commandList->GetCommandAllocator());
	m_inactiveList.push_back(commandList);
	commandList = nullptr;
}

void DX12ManagerCommandList::ExecuteWaitingList(UINT queueIndex)
{
	m_waitingLists[queueIndex].ExecuteWaitingList((*m_pCommandQueues)[queueIndex].get(), m_inactiveList);
}

void DX12ManagerCommandList::ExecuteAllWaitingList()
{
	for (size_t i = 0; i < m_waitingLists.size(); i++)
	{
		ExecuteWaitingList(i);
	}
}

shared_ptr<DX12CommandList> DX12ManagerCommandList::GetCommandList()
{
	shared_ptr<DX12CommandList> list;
	if (m_inactiveList.empty())
	{
		list = make_shared<DX12CommandList>();
		list->Initialize(m_device, m_nodeMask, m_type, m_allocatorManager->GetAllocator(m_type));
	}
	else
	{
		list = m_inactiveList.back();
		m_inactiveList.pop_back();
		list->Reset(m_allocatorManager->GetAllocator(m_type));
	}
	return list;
}

void DX12ManagerCommandList::WaitingList::AddCommandList(const shared_ptr<DX12CommandList>& commandList)
{
	m_waitingList.push_back(commandList);
	m_rawList.push_back(commandList->GetBase()->GetInterface());
	m_waitingAllocator.push_back(commandList->GetCommandAllocator());
}

void DX12ManagerCommandList::WaitingList::ExecuteWaitingList(DX12CommandQueue* commandQueue, std::vector<shared_ptr<DX12CommandList>>& inactiveList)
{
	commandQueue->ExecuteCommandLists(m_rawList.size(), m_rawList.data());
	commandQueue->SetActiveAllocators(m_waitingAllocator);
	std::move(m_waitingList.begin(), m_waitingList.end(), std::back_inserter(inactiveList));
	m_waitingList.clear();
	m_waitingAllocator.clear();
	m_rawList.clear();
}
