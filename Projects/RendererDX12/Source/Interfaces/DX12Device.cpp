#include "RendererDX12.h"
#include "Interfaces/DX12Device.h"

DX12Device::DX12Device()
{
}


void DX12Device::Initialize(DX12CommandAllocatorManager* manager, UINT directQueues, UINT copyQueues, UINT computeQueues, D3D_FEATURE_LEVEL featureLevel, UINT adapterID, UINT nodeMask)
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

	if (directQueues)
	{
		m_directList = make_unique<DX12CommandListManager>();
		m_directList->Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_DIRECT, directQueues);

		for (int i = 0; i < directQueues; i++)
		{
			m_directQueue.push_back(make_unique<DX12CommandQueue>());
			m_directQueue[i]->Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_DIRECT);
		}
	}

	if (computeQueues)
	{
		m_computeList = make_unique<DX12CommandListManager>();
		m_computeList->Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_DIRECT, computeQueues);
		for (int i = 0; i < copyQueues; i++)
		{
			m_copyQueue.push_back(make_unique<DX12CommandQueue>());
			m_copyQueue[i]->Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_COPY);
		}
	}
	if (copyQueues)
	{
		m_copyList = make_unique<DX12CommandListManager>();
		m_copyList->Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_DIRECT, copyQueues);
		for (int i = 0; i < computeQueues; i++)
		{
			m_computeQueue.push_back(make_unique<DX12CommandQueue>());
			m_computeQueue[i]->Initialize(this, manager, D3D12_COMMAND_LIST_TYPE_COMPUTE);
		}
	}

}

void DX12Device::ExecuteCommandLists(UINT numCommandLists, ID3D12CommandList*const* commandLists, UINT queueIndex)
{
	switch (commandLists[0]->GetType())
	{
	case D3D12_COMMAND_LIST_TYPE_BUNDLE:
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		if (m_directQueue.size() <= queueIndex - 1)
			m_directQueue[queueIndex]->GetCommandQueue()->ExecuteCommandLists(numCommandLists, commandLists);
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		if (m_computeQueue.size() <= queueIndex - 1)
			m_computeQueue[queueIndex]->GetCommandQueue()->ExecuteCommandLists(numCommandLists, commandLists);
	case D3D12_COMMAND_LIST_TYPE_COPY:
		if (m_computeQueue.size() <= queueIndex - 1)
			m_computeQueue[queueIndex]->GetCommandQueue()->ExecuteCommandLists(numCommandLists, commandLists);
		break;
	default:
		break;
	}

}

void DX12Device::ExecuteAllCommandListManager()
{
	if (m_directList)
		m_directList->ExecuteAllLists();
	if (m_computeList)
		m_computeList->ExecuteAllLists();
	if (m_copyList)
		m_copyList->ExecuteAllLists();
}

void DX12Device::SignalAllQueues()
{
	for (const unique_ptr<DX12CommandQueue>& queue : m_directQueue)
	{
		queue->SignalGPU();
	}
	for (const unique_ptr<DX12CommandQueue>& queue : m_computeQueue)
	{
		queue->SignalGPU();
	}
	for (const unique_ptr<DX12CommandQueue>& queue : m_copyQueue)
	{
		queue->SignalGPU();
	}
}


void DX12Device::SyncAllQueues()
{
	for (const unique_ptr<DX12CommandQueue>& queue : m_directQueue)
	{
		queue->SyncQueue(INFINITE);
	}
	for (const unique_ptr<DX12CommandQueue>& queue : m_computeQueue)
	{
		queue->SyncQueue(INFINITE);
	}
	for (const unique_ptr<DX12CommandQueue>& queue : m_copyQueue)
	{
		queue->SyncQueue(INFINITE);
	}
}


DX12CommandQueue* DX12Device::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type, UINT index)
{
	DX12CommandQueue* queue = nullptr;
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		queue = m_directQueue[index].get();
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		queue = m_computeQueue[index].get();
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		queue = m_copyQueue[index].get();
		break;
	}
	return queue;
}

void DX12Device::ExecuteCommandListManager(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
{
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		return m_directList->ExecuteList(queueIndex);
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		return m_computeList->ExecuteList(queueIndex);
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		return m_copyList->ExecuteList(queueIndex);
		break;
	default:
		break;
	}
}

shared_ptr<DX12CommandList> DX12Device::GetCommandList(D3D12_COMMAND_LIST_TYPE type, UINT queuePreference)
{
	shared_ptr<DX12CommandList> list = nullptr;
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		if (m_directList)
			list = m_directList->GetCommandList(queuePreference);
		break;
	case D3D12_COMMAND_LIST_TYPE_BUNDLE:
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		if (m_computeList)
			list = m_computeList->GetCommandList(queuePreference);
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		if (m_copyList)
			list = m_copyList->GetCommandList(queuePreference);
		break;
	default:
		break;
	}
	return list;
}