#pragma once
#include "DX12Header.h"
class DX12Device;
class DX12CommandList;
class DX12CommandAllocatorManager;

class DX12CommandQueue
{
public:
	DX12CommandQueue();
	void Initialize(DX12Device* device, D3D12_COMMAND_LIST_TYPE commandListType);

	void SignalGPU();
	void SignalCPU();
	void SyncQueue(DWORD milliseconds);

	void ResetFenceValue() { m_fenceValue = 0; }


	inline ID3D12CommandQueue* GetCommandQueue() { return m_commandQueue.Get(); }
	inline ID3D12Fence* GetFence() { return m_fence.Get(); }
	inline UINT GetFenceValue() { return m_fenceValue; }
	inline bool IsSynced() { return m_isSynced; }
private:
	DX12Device* m_device;

	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12Fence> m_fence;
	UINT m_fenceValue;
	HANDLE m_fenceEvent;

	bool m_isSynced;
};