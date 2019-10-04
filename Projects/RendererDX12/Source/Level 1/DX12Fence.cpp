#include "RendererDX12.h"
#include "Level 1/DX12Fence.h"

DX12Fence::DX12Fence() :
	fenceValue(0),
	highestSyncedValue(0)
{
}

void DX12Fence::Initialize(ID3D12Device* device, D3D12_FENCE_FLAGS flags)
{
	device->CreateFence(0, flags, IID_PPV_ARGS(m_fence.GetAddressOf()));
}

void DX12Fence::Reset()
{
	fenceValue = highestSyncedValue = 0;
	m_fence->Signal(fenceValue);
}
