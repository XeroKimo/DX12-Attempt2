#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseCommandQueue.h"
class DX12CommandAllocator;
class DX12ManagerCommandAllocator;

class DX12CommandQueue
{
public:
	DX12CommandQueue();
	void Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType, DX12ManagerCommandAllocator* allocatorManager);

	void Signal();
	void StallQueue(DX12BaseCommandQueue* queue);
	void SyncQueue(DWORD milliseconds);

	void SetActiveAllocators(std::vector<shared_ptr<DX12CommandAllocator>> allocator);
	inline void ExecuteCommandLists(UINT numList, ID3D12CommandList* const* list) { m_commandQueue.GetInterface()->ExecuteCommandLists(numList, list); }
	inline void SetActiveAllocator(shared_ptr<DX12CommandAllocator> allocator) { m_runningAllocators.push_back(allocator); }
	inline DX12BaseCommandQueue* GetBase() { return &m_commandQueue; }
private:
	DX12ManagerCommandAllocator* m_allocatorManager;
	DX12BaseCommandQueue m_commandQueue;

	std::vector<shared_ptr<DX12CommandAllocator>> m_runningAllocators;
};