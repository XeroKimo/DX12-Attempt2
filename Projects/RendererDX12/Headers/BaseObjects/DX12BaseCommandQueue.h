#pragma once
#include "DX12Header.h"

class DX12BaseCommandQueue
{
public:
	DX12BaseCommandQueue();
	void Initialize(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE commandListType);

	void Signal();
	void SyncQueue(DWORD milliseconds);

    inline void ResetFenceValue() { m_fenceValue = 0; m_fence->Signal(0); }
	inline ID3D12CommandQueue* GetCommandQueue() { return m_commandQueue.Get(); }
	inline ID3D12Fence* GetFence() { return m_fence.Get(); }
	inline UINT GetFenceValue() { return m_fenceValue; }
private:

	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12Fence> m_fence;
	UINT m_fenceValue;
	HANDLE m_fenceEvent;
};