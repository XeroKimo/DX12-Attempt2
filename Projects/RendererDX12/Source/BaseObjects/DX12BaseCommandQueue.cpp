#include "RendererDX12.h"

DX12BaseCommandQueue::DX12BaseCommandQueue() :
	m_fenceValue(0),
    m_fenceEvent(nullptr)
{
}

void DX12BaseCommandQueue::Signal()
{
    m_fenceValue++;
	m_commandQueue->Signal(m_fence.Get(), m_fenceValue);
}

void DX12BaseCommandQueue::SyncQueue(DWORD milliseconds)
{
	if (m_fence->GetCompletedValue() < m_fenceValue)
	{
		m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, milliseconds);
	}
}

void DX12BaseCommandQueue::Initialize(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE commandListType)
{
	HRESULT hr;

	D3D12_COMMAND_QUEUE_DESC cqDesc;
	cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqDesc.NodeMask = device->GetNodeCount();
	cqDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cqDesc.Type = commandListType;

	hr = device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(m_commandQueue.GetAddressOf()));
	assert(SUCCEEDED(hr));

	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()));
	assert(SUCCEEDED(hr));

	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}
