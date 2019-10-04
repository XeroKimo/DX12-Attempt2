#pragma once
#include "DX12Header.h"

class DX12Fence
{
public:
	DX12Fence();
	void Initialize(ID3D12Device* device, D3D12_FENCE_FLAGS flags = D3D12_FENCE_FLAG_NONE);

	void Reset();

	inline ID3D12Fence* GetInterface() { return m_fence.Get(); }
public:
	UINT64 fenceValue;
	UINT64 highestSyncedValue;

private:
	ComPtr<ID3D12Fence> m_fence;
};