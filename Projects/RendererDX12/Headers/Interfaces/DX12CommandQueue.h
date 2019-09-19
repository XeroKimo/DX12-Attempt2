#pragma once
#include "DX12Header.h"
#include "BaseObjects/DX12BaseCommandQueue.h"
class DX12CommandAllocator;
class DX12ManagerCommandAllocator;

class DX12CommandQueue
{
public:
	DX12CommandQueue();
	void Initialize(ID3D12Device* device, DX12ManagerCommandAllocator* allocatorManager, D3D12_COMMAND_LIST_TYPE commandListType);

	void Signal();
	void StallQueue(DX12BaseCommandQueue* queue);
	void SyncQueue(DWORD milliseconds);

	void SetActiveAllocators(std::vector<shared_ptr<DX12CommandAllocator>> allocator);
	inline void SetActiveAllocator(shared_ptr<DX12CommandAllocator> allocator) { m_runningAllocators.push_back(allocator); }
	inline DX12BaseCommandQueue* GetBaseCommandQueue() { return &m_commandQueue; }
private:
	DX12ManagerCommandAllocator* m_allocatorManager;
	DX12BaseCommandQueue m_commandQueue;

	std::vector<shared_ptr<DX12CommandAllocator>> m_runningAllocators;
};