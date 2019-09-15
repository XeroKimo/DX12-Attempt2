#include "RendererDX12.h"
#include "Interfaces/DX12CommandQueue.h"

DX12CommandQueue::DX12CommandQueue()
{
}

void DX12CommandQueue::Initialize(ID3D12Device* device, DX12ManagerCommandAllocator* allocatorManager, D3D12_COMMAND_LIST_TYPE commandListType)
{
	m_commandQueue.Initialize(device, commandListType);
	m_allocatorManager = allocatorManager;
}

void DX12CommandQueue::SignalGPU()
{
	if (m_runningAllocators.empty())
		return;
	m_commandQueue.SignalGPU();
}

void DX12CommandQueue::StallQueue(DX12BaseCommandQueue* queue)
{
	m_commandQueue.GetCommandQueue()->Wait(queue->GetFence(), queue->GetFenceValue());
}

void DX12CommandQueue::SyncQueue(DWORD milliseconds)
{
	if (m_runningAllocators.empty())
		return;
	m_commandQueue.SyncQueue(milliseconds);
	m_allocatorManager->ResetAllocators(m_runningAllocators);
	m_runningAllocators.clear();
}

void DX12CommandQueue::SetActiveAllocators(std::vector<shared_ptr<DX12CommandAllocator>> allocator)
{
	std::copy(allocator.begin(), allocator.end(), std::back_inserter(m_runningAllocators));
}