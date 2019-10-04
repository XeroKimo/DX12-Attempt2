#include "RendererDX12.h"
#include "Level 3/DX12CommandQueue.h"

DX12CommandQueue::DX12CommandQueue() :
	m_allocatorManager(nullptr),
	m_highestSignaledHistory(0),
	m_highestSyncedSignal(0),
	m_signalHistory()
{
}

void DX12CommandQueue::Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType, DX12ManagerCommandAllocator* allocatorManager)
{
	m_commandQueue.Initialize(device, nodeMask, commandListType);
	m_allocatorManager = allocatorManager;
	m_signalHistory.fill(0);
}

void DX12CommandQueue::Signal()
{
	if (m_runningAllocators.empty())
		return;
	m_commandQueue.Signal();
	m_signalHistory[(m_commandQueue.GetFenceValue() - 1) % MAX_SIGNAL_HISTORY] = m_runningAllocators.size();
}

void DX12CommandQueue::SyncQueue(DWORD milliseconds)
{
	if (m_runningAllocators.empty())
		return;
	m_commandQueue.SyncQueue(milliseconds);
	m_allocatorManager->ResetAllocators(m_runningAllocators);
	m_runningAllocators.clear();
	Reset();
}

void DX12CommandQueue::SyncQueue(DWORD milliseconds, UINT64 fenceValue)
{
    if (m_runningAllocators.empty())
        return;

	if (fenceValue < m_highestSyncedSignal)
		return;

	UINT64 valueToSync = (fenceValue == 0 || fenceValue > m_commandQueue.GetFenceValue()) ? m_commandQueue.GetFenceValue() : fenceValue;
	unsigned int roundedFenceValue = (valueToSync - 1) % MAX_SIGNAL_HISTORY;
	size_t iteratorOffset = m_signalHistory[roundedFenceValue] - m_highestSignaledHistory;

	if (iteratorOffset < 1)
	{
		assert(false); // Queue will fail to sync
		return;
	}
	m_commandQueue.SyncQueue(milliseconds, valueToSync);
	m_highestSyncedSignal = valueToSync;

	std::vector<unique_ptr<DX12CommandAllocator>> allocatorsToReset;
	auto it = m_runningAllocators.begin() + iteratorOffset;

	std::move(m_runningAllocators.begin(), it, std::back_inserter(allocatorsToReset));
	m_allocatorManager->ResetAllocators(allocatorsToReset);
	m_runningAllocators.erase(m_runningAllocators.begin(), it);

	if (m_runningAllocators.empty())
		Reset();
	else
		m_highestSignaledHistory = m_signalHistory[roundedFenceValue];
}

void DX12CommandQueue::SetActiveAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocator)
{
	std::move(allocator.begin(), allocator.end(), std::back_inserter(m_runningAllocators));
}

void DX12CommandQueue::Reset()
{
	m_commandQueue.ResetFenceValue();
	m_highestSignaledHistory = 0;
	m_highestSyncedSignal = 0;
}
