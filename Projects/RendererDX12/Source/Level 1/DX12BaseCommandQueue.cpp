#include "RendererDX12.h"
#include "Level 1/DX12BaseCommandQueue.h"

DX12BaseCommandQueue::DX12BaseCommandQueue() :
	m_fenceValue(0),
    m_fenceEvent(nullptr),
	m_type(D3D12_COMMAND_LIST_TYPE_DIRECT)
{
}

void DX12BaseCommandQueue::Signal()
{
    m_fenceValue++;
	m_commandQueue->Signal(m_fence.Get(), m_fenceValue);
}

void DX12BaseCommandQueue::StallQueue(ID3D12Fence* fence, UINT64 fenceValue)
{
	m_commandQueue->Wait(fence, fenceValue);
}

void DX12BaseCommandQueue::SyncQueue(DWORD milliseconds, UINT64 fenceValue)
{
	UINT64& valueToSync = (fenceValue == FENCE_SIGNAL_VALUE_MAX || fenceValue > m_fenceValue) ? m_fenceValue : fenceValue;
	if (m_fence->GetCompletedValue() < valueToSync)
	{
		m_fence->SetEventOnCompletion(valueToSync, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, milliseconds);
	}
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

	hr = device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()));
	assert(SUCCEEDED(hr));

	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}
