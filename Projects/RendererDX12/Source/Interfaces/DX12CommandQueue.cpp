#include "RendererDX12.h"
#include "Interfaces/DX12CommandQueue.h"

DX12CommandQueue::DX12CommandQueue() :
	m_fenceValue(0),
	m_device(nullptr)
{
}

void DX12CommandQueue::SignalGPU()
{
	m_commandQueue->Signal(m_fence.Get(), m_fenceValue);
}

void DX12CommandQueue::SignalCPU()
{
	m_fence->Signal(m_fenceValue);
}

void DX12CommandQueue::SyncQueue(DWORD milliseconds)
{
	if (m_fence->GetCompletedValue() < m_fenceValue)
	{
		m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, milliseconds);
	}
	if (!m_runningAllocators.empty())
		m_allocatorManager->ResetAllocators(m_runningAllocators);
	m_runningAllocators.clear();
	m_fenceValue++;
}



void DX12CommandQueue::Initialize(DX12Device* device, DX12CommandAllocatorManager* allocatorManager, D3D12_COMMAND_LIST_TYPE commandListType)
{
	m_device = device;
	m_allocatorManager = allocatorManager;

	HRESULT hr;

	D3D12_COMMAND_QUEUE_DESC cqDesc;
	cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqDesc.NodeMask = device->GetNodeMask();
	cqDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cqDesc.Type = commandListType;

	hr = device->GetDevice()->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(m_commandQueue.GetAddressOf()));
	if (FAILED(hr))
		assert(false);

	hr = device->GetDevice()->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()));
	if (FAILED(hr))
		assert(false);

	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}
