#pragma once
#include "DX12Header.h"
#include "Level 2/DX12BaseCommandQueue.h"
class DX12CommandAllocator;
class DX12ManagerCommandAllocator;

#define MAX_SIGNAL_HISTORY 64

class DX12CommandQueue
{
public:
	DX12CommandQueue();
	void Initialize(DX12BaseDevice* device, D3D12_COMMAND_LIST_TYPE commandListType, DX12ManagerCommandAllocator* allocatorManager);

	UINT64 Signal();
	void SyncQueue(DWORD milliseconds);
	void SyncQueue(DWORD milliseconds, UINT64 fenceValue);

	void SetActiveAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocator);
	void Reset();
	inline void StallQueue(DX12Fence* fence, UINT64 fenceValue) { if (fenceValue <= fence->fenceValue) m_commandQueue.StallQueue(fence, fenceValue); }
	inline void SetActiveAllocator(unique_ptr<DX12CommandAllocator>& allocator) { m_runningAllocators.push_back(std::move(allocator)); }

	inline DX12Fence* GetFence() { return m_commandQueue.GetFence(); }

	inline ID3D12CommandQueue* GetInterface() { return m_commandQueue.GetInterface(); }
	inline ID3D12Fence* GetFenceInterface() { return m_commandQueue.GetFenceInterface(); }
private:
	DX12ManagerCommandAllocator* m_allocatorManager;
	DX12BaseCommandQueue m_commandQueue;

	std::vector<unique_ptr<DX12CommandAllocator>> m_runningAllocators;

	std::array<size_t, MAX_SIGNAL_HISTORY> m_allocatorSizeHistory;
	size_t m_highestSignaledSize;
};