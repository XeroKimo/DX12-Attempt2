#include "RendererDX12.h"
#include "Level 2/DX12BaseCommandQueue.h"

DX12BaseCommandQueue::DX12BaseCommandQueue(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType) :
    m_fence(device)
{
    HRESULT hr;

    D3D12_COMMAND_QUEUE_DESC cqDesc;
    cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cqDesc.NodeMask = nodeMask;
    cqDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cqDesc.Type = commandListType;

    m_type = commandListType;

    hr = device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(m_commandQueue.GetAddressOf()));
    assert(SUCCEEDED(hr));

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

UINT64 DX12BaseCommandQueue::Signal()
{
	return Signal(&m_fence); 
}

UINT64 DX12BaseCommandQueue::Signal(DX12Fence* fence)
{
	fence->fenceValue++;
	m_commandQueue->Signal(fence->GetInterface(), fence->fenceValue);
	return m_fence.fenceValue;
}

void DX12BaseCommandQueue::StallQueue(DX12Fence* fence, UINT64 fenceValue)
{
	m_commandQueue->Wait(fence->GetInterface(), fenceValue);
}

void DX12BaseCommandQueue::SyncQueue(DWORD milliseconds, UINT64 fenceValue)
{
	SyncFence(milliseconds, &m_fence, fenceValue);
}

void DX12BaseCommandQueue::SyncFence(DWORD milliseconds, DX12Fence* fence, UINT64 fenceValue)
{
	if (fence->GetInterface()->GetCompletedValue() < fenceValue)
	{
		fence->GetInterface()->SetEventOnCompletion(fenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, milliseconds);
	}
	fence->highestSyncedValue = fenceValue;
}