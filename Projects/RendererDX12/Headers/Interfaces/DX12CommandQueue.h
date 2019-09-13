#pragma once
#include "DX12Header.h"
class DX12Device;
class DX12CommandList;
class DX12CommandAllocator;
class DX12MCommandAllocatorManager;

class DX12CommandQueue
{
public:
	DX12CommandQueue();
	void Initialize(ID3D12Device* device, DX12MCommandAllocatorManager* allocatorManager, D3D12_COMMAND_LIST_TYPE commandListType);

	void SignalGPU();
	void SignalCPU();
	void SyncQueue(DWORD milliseconds);

	inline void SetActiveAllocator(shared_ptr<DX12CommandAllocator> allocator) { m_runningAllocators.push_back(allocator); }
	void SetActiveAllocator(std::vector<shared_ptr<DX12CommandAllocator>> allocator);
	inline void ResetFenceValue() { m_fenceValue = 0; }
	inline ID3D12CommandQueue* GetCommandQueue() { return m_commandQueue.Get(); }
	inline ID3D12Fence* GetFence() { return m_fence.Get(); }
	inline UINT GetFenceValue() { return m_fenceValue; }
private:
	DX12MCommandAllocatorManager* m_allocatorManager;

	ComPtr<ID3D12CommandQueue> m_commandQueue;
	std::vector<shared_ptr<DX12CommandAllocator>> m_runningAllocators;
	ComPtr<ID3D12Fence> m_fence;
	UINT m_fenceValue;
	HANDLE m_fenceEvent;
};