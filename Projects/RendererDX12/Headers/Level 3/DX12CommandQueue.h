#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseCommandQueue.h"
class DX12CommandAllocator;
class DX12ManagerCommandAllocator;

#define MAX_SIGNAL_HISTORY 64

class DX12CommandQueue
{
public:
	DX12CommandQueue();
	void Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType, DX12ManagerCommandAllocator* allocatorManager);

	void Signal();
	void StallQueue(ID3D12Fence* fence, UINT64 fenceValue) { m_commandQueue.StallQueue(fence, fenceValue); }
	void SyncQueue(DWORD milliseconds);
	void SyncQueue(DWORD milliseconds, UINT64 fenceValue);

	void SetActiveAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocator);
	void Reset();
	inline void ExecuteCommandLists(UINT numList, ID3D12CommandList* const* list) { m_commandQueue.GetInterface()->ExecuteCommandLists(numList, list); }
	inline void SetActiveAllocator(unique_ptr<DX12CommandAllocator>& allocator) { m_runningAllocators.push_back(std::move(allocator)); }
	//inline DX12BaseCommandQueue* GetBase() { return &m_commandQueue; }
	inline ID3D12CommandQueue* GetInterface() { return m_commandQueue.GetInterface(); }
	inline ID3D12Fence* GetFence() { return m_commandQueue.GetFence(); }
	inline UINT64 GetFenceValue() { return m_commandQueue.GetFenceValue(); }
private:
	DX12ManagerCommandAllocator* m_allocatorManager;
	DX12BaseCommandQueue m_commandQueue;

	std::vector<unique_ptr<DX12CommandAllocator>> m_runningAllocators;

	UINT64 m_highestSyncedSignal;
	std::array<size_t, MAX_SIGNAL_HISTORY> m_signalHistory;
	size_t m_highestSignaledHistory;
};