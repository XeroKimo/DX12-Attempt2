#include "RendererDX12.h"
#include "Interfaces/DX12Device.h"

DX12Device::DX12Device()
{
}


void DX12Device::Initialize(DX12Renderer* renderer, DX12CommandAllocatorManager* manager, UINT directQueues, UINT copyQueues, UINT computeQueues, D3D_FEATURE_LEVEL featureLevel, UINT adapterID, UINT nodeMask)
{

	HRESULT hr;
	ComPtr<IDXGIFactory2> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(factory.GetAddressOf()));
	if (FAILED(hr))
		assert(false);


	ComPtr<IDXGIAdapter1> adapter;
	hr = factory->EnumAdapters1(adapterID, adapter.GetAddressOf());
	if (FAILED(hr))
		assert(false);

	hr = adapter->QueryInterface(m_adapter.GetAddressOf());
	if (FAILED(hr))
		assert(false);

	hr = D3D12CreateDevice(m_adapter.Get(), featureLevel, IID_PPV_ARGS(m_device.GetAddressOf()));
	if (FAILED(hr))
		assert(false);

	m_nodeMask = nodeMask;

	m_directQueue.reserve(directQueues);
	m_copyQueue.reserve(copyQueues);
	m_computeQueue.reserve(computeQueues);

	for (int i = 0; i < directQueues; i++)
	{
		m_directQueue.push_back(DX12CommandQueue());
		m_directQueue[i].Initialize(this, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	for (int i = 0; i < copyQueues; i++)
	{
		m_copyQueue.push_back(DX12CommandQueue());
		m_copyQueue[i].Initialize(this, D3D12_COMMAND_LIST_TYPE_COPY);
	}
	for (int i = 0; i < computeQueues; i++)
	{
		m_computeQueue.push_back(DX12CommandQueue());
		m_computeQueue[i].Initialize(this, D3D12_COMMAND_LIST_TYPE_COMPUTE);
	}

	m_computeList.Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_COMPUTE);
	m_directList.Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_DIRECT);
	m_copyList.Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_COPY);

}

void DX12Device::ExecuteCommandLists(UINT numCommandLists, ID3D12CommandList*const* commandLists)
{
	switch (commandLists[0]->GetType())
	{
	case D3D12_COMMAND_LIST_TYPE_BUNDLE:
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		for (DX12CommandQueue& queue : m_directQueue)
		{
			if (queue.IsSynced())
				queue.GetCommandQueue()->ExecuteCommandLists(numCommandLists, commandLists);
		}
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:		
		for (DX12CommandQueue& queue : m_computeQueue)
		{
			if (queue.IsSynced())
				queue.GetCommandQueue()->ExecuteCommandLists(numCommandLists, commandLists);
		}
	case D3D12_COMMAND_LIST_TYPE_COPY:
		for (DX12CommandQueue& queue : m_copyQueue)
		{
			if (queue.IsSynced())
				queue.GetCommandQueue()->ExecuteCommandLists(numCommandLists, commandLists);
		}
		break;
	default:
		break;
	}

}

void DX12Device::ExecuteAllLists()
{
	m_directList.ExecuteList();
	m_copyList.ExecuteList();
	m_computeList.ExecuteList();
}

void DX12Device::SignalAllQueues()
{
	for (DX12CommandQueue& queue : m_directQueue)
	{
		queue.SignalGPU();
	}
	for (DX12CommandQueue& queue : m_computeQueue)
	{
		queue.SignalGPU();
	}
	for (DX12CommandQueue& queue : m_copyQueue)
	{
		queue.SignalGPU();
	}
}


void DX12Device::SyncAllQueues()
{
	for (DX12CommandQueue& queue : m_directQueue)
	{
		queue.SyncQueue(INFINITE);
	}
	for (DX12CommandQueue& queue : m_computeQueue)
	{
		queue.SyncQueue(INFINITE);
	}
	for (DX12CommandQueue& queue : m_copyQueue)
	{
		queue.SyncQueue(INFINITE);
	}
}


DX12CommandQueue* DX12Device::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type, UINT index)
{
	DX12CommandQueue* queue = nullptr;
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		queue = &m_directQueue[index];
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		queue = &m_computeQueue[index];
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		queue = &m_copyQueue[index];
		break;
	}
	return queue;
}

shared_ptr<DX12CommandList> DX12Device::GetCommandList(D3D12_COMMAND_LIST_TYPE type)
{
	shared_ptr<DX12CommandList> list;
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		list = m_directList.GetCommandList();
		break;
	case D3D12_COMMAND_LIST_TYPE_BUNDLE:
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		list = m_computeList.GetCommandList();
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		list = m_copyList.GetCommandList();
		break;
	default:
		break;
	}
	return list;
}