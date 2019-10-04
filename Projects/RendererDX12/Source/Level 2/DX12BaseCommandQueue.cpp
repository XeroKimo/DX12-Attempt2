#include "RendererDX12.h"
#include "Level 2/DX12BaseCommandQueue.h"

DX12BaseCommandQueue::DX12BaseCommandQueue() :
    m_fenceEvent(nullptr),
	m_type(D3D12_COMMAND_LIST_TYPE_DIRECT)
{
}

void DX12BaseCommandQueue::Signal()
{
    m_fence.fenceValue++;
	m_commandQueue->Signal(m_fence.GetInterface(), m_fence.fenceValue);
}

void DX12BaseCommandQueue::Signal(DX12Fence* fence)
{
	fence->fenceValue++;
	m_commandQueue->Signal(fence->GetInterface(), fence->fenceValue);
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
	UINT64& valueToSync = (fenceValue == FENCE_SIGNAL_VALUE_MAX || fenceValue > fence->fenceValue) ? fence->fenceValue : fenceValue;
	if (fence->GetInterface()->GetCompletedValue() < valueToSync)
	{
		fence->GetInterface()->SetEventOnCompletion(valueToSync, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, milliseconds);
	}
	fence->highestSyncedValue = valueToSync;
}

void DX12BaseCommandQueue::Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType)
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

	m_fence.Initialize(device);

	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}
