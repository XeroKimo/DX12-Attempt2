#include "RendererDX12.h"
#include "Level 3/DX12CommandQueue.h"

DX12CommandQueue::DX12CommandQueue() :
	m_allocatorManager(nullptr),
	m_highestSignal(0)
{
}

void DX12CommandQueue::Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType, DX12ManagerCommandAllocator* allocatorManager)
{
	m_commandQueue.Initialize(device, nodeMask, commandListType);
	m_allocatorManager = allocatorManager;
}

void DX12CommandQueue::Signal()
{
	if (m_runningAllocators.empty())
		return;
	m_commandQueue.Signal();
	m_signalHistory.push_back(m_runningAllocators.size());
}

void DX12CommandQueue::SyncQueue(DWORD milliseconds, UINT64 fenceValue)
{
    if (m_runningAllocators.empty())
        return;

	UINT64 valueToSync = (fenceValue == 0) ? m_commandQueue.GetFenceValue() : fenceValue;
	m_commandQueue.SyncQueue(milliseconds, valueToSync);

	std::vector<unique_ptr<DX12CommandAllocator>> allocatorsToReset;
	size_t iteratorOffset = m_signalHistory[valueToSync-1] - m_highestSignal;
	auto it = m_runningAllocators.begin() +  iteratorOffset;

	std::move(m_runningAllocators.begin(), it, std::back_inserter(allocatorsToReset));
	m_allocatorManager->ResetAllocators(allocatorsToReset);
	m_runningAllocators.erase(m_runningAllocators.begin(), it);

	if (m_runningAllocators.empty())
	{
		m_signalHistory.clear();
		m_highestSignal = 0;
	}
	else
		m_highestSignal = m_signalHistory[valueToSync-1];
}

void DX12CommandQueue::SetActiveAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocator)
{
	std::move(allocator.begin(), allocator.end(), std::back_inserter(m_runningAllocators));
}
