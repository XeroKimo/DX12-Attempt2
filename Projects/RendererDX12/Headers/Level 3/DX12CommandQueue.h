#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseCommandQueue.h"
class DX12CommandAllocator;
class DX12ManagerCommandAllocator;

class DX12CommandQueue
{
	using SignalHistory = size_t;
public:
	DX12CommandQueue();
	void Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType, DX12ManagerCommandAllocator* allocatorManager);

	void Signal();
	void StallQueue(ID3D12Fence* fence, UINT64 fenceValue) { m_commandQueue.StallQueue(fence, fenceValue); }
	void SyncQueue(DWORD milliseconds, UINT64 fenceValue = 0);

	void SetActiveAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocator);
    inline void ResetFenceValue() { m_commandQueue.ResetFenceValue(); }
	inline void ExecuteCommandLists(UINT numList, ID3D12CommandList* const* list) { m_commandQueue.GetInterface()->ExecuteCommandLists(numList, list); }
	inline void SetActiveAllocator(unique_ptr<DX12CommandAllocator>& allocator) { m_runningAllocators.push_back(std::move(allocator)); }
	inline DX12BaseCommandQueue* GetBase() { return &m_commandQueue; }
private:
	DX12ManagerCommandAllocator* m_allocatorManager;
	DX12BaseCommandQueue m_commandQueue;

	std::vector<unique_ptr<DX12CommandAllocator>> m_runningAllocators;
	std::vector<SignalHistory> m_signalHistory;
	SignalHistory m_highestSignal;
};