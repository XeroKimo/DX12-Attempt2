#include "RendererDX12.h"
#include "Level 3/DX12CommandQueue.h"

DX12CommandQueue::DX12CommandQueue() :
	m_allocatorManager(nullptr)
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
}

void DX12CommandQueue::StallQueue(DX12BaseCommandQueue* queue)
{
	m_commandQueue.GetInterface()->Wait(queue->GetFence(), queue->GetFenceValue());
}

void DX12CommandQueue::SyncQueue(DWORD milliseconds)
{
	if (m_runningAllocators.empty())
		return;
	m_commandQueue.SyncQueue(milliseconds);
	m_allocatorManager->ResetAllocators(m_runningAllocators);
}

void DX12CommandQueue::SetActiveAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocator)
{
	std::move(allocator.begin(), allocator.end(), std::back_inserter(m_runningAllocators));
}
