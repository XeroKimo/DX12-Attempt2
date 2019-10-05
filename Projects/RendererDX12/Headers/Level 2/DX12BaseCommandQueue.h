#pragma once
#include "DX12Header.h"
#include "Level 1/DX12Fence.h"

#define FENCE_SIGNAL_VALUE_MAX UINT64_MAX

class DX12BaseCommandQueue
{
public:
	DX12BaseCommandQueue();
	void Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType);

	UINT64 Signal();
	UINT64 Signal(DX12Fence* fence);
	void StallQueue(DX12Fence* fence, UINT64 fenceValue);
	void SyncQueue(DWORD milliseconds, UINT64 fenceValue = FENCE_SIGNAL_VALUE_MAX);
	void SyncFence(DWORD milliseconds, DX12Fence* fence, UINT64 fenceValue = FENCE_SIGNAL_VALUE_MAX);

	inline bool FenceSyncable(DX12Fence* fence, UINT64 fenceValue) { return (fenceValue < m_fence.fenceValue || fenceValue > m_fence.highestSyncedValue); }
	inline void ResetFence() { m_fence.Reset(); }
	inline ID3D12CommandQueue* GetInterface() { return m_commandQueue.Get(); }
	inline DX12Fence* GetFence() { return &m_fence; }
	inline ID3D12Fence* GetFenceInterface() { return m_fence.GetInterface(); }
private:

	ComPtr<ID3D12CommandQueue> m_commandQueue;
	DX12Fence m_fence;
	//ComPtr<ID3D12Fence> m_fence;
    D3D12_COMMAND_LIST_TYPE m_type;
	HANDLE m_fenceEvent;
};