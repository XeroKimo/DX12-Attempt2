#include "RendererDX12.h"
#include "Level 4/DX12Device.h"

DX12Device::DX12Device()
{
}

void DX12Device::Initialize(D3D_FEATURE_LEVEL featureLevel, UINT adapterID, DX12ManagerCommandAllocator* manager, UINT directQueues, UINT computeQueues, UINT copyQueues)
{
    m_device.Initialize(featureLevel, adapterID);

    m_directQueue.reserve(directQueues);
    m_copyQueue.reserve(copyQueues);
    m_computeQueue.reserve(computeQueues);

    if (directQueues)
    {
        for (UINT i = 0; i < directQueues; i++)
        {
            m_directQueue.push_back(make_unique<DX12CommandQueue>());
            m_directQueue[i]->Initialize(m_device.GetInterface(), m_device.GetNodeMask(), D3D12_COMMAND_LIST_TYPE_DIRECT, manager);
        }
		m_directList = make_unique<DX12ManagerCommandList>();
		m_directList->Initialize(m_device.GetInterface(), m_device.GetNodeMask(), D3D12_COMMAND_LIST_TYPE_DIRECT, &m_directQueue, manager);
    }

    if (computeQueues)
    {
        for (UINT i = 0; i < computeQueues; i++)
        {
            m_computeQueue.push_back(make_unique<DX12CommandQueue>());
            m_computeQueue[i]->Initialize(m_device.GetInterface(), m_device.GetNodeMask(), D3D12_COMMAND_LIST_TYPE_COMPUTE, manager);
        }
		m_computeList = make_unique<DX12ManagerCommandList>();
		m_computeList->Initialize(m_device.GetInterface(), m_device.GetNodeMask(), D3D12_COMMAND_LIST_TYPE_COMPUTE, &m_computeQueue, manager);
    }
    if (copyQueues)
    {
        for (UINT i = 0; i < copyQueues; i++)
        {
            m_copyQueue.push_back(make_unique<DX12CommandQueue>());
            m_copyQueue[i]->Initialize(m_device.GetInterface(), m_device.GetNodeMask(), D3D12_COMMAND_LIST_TYPE_COPY, manager);
        }
		m_copyList = make_unique<DX12ManagerCommandList>();
		m_copyList->Initialize(m_device.GetInterface(), m_device.GetNodeMask(), D3D12_COMMAND_LIST_TYPE_COPY, &m_copyQueue, manager);
    }
}

void DX12Device::CloseCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex)
{
	DX12ManagerCommandList* commandListManager = GetCommandListManager(commandList->GetType());
	if (commandListManager != nullptr)
		commandListManager->CloseCommandList(commandList, queueIndex);
}

void DX12Device::ExecuteCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex)
{
	DX12ManagerCommandList* commandListManager = GetCommandListManager(commandList->GetType());
	if (commandListManager != nullptr)
		commandListManager->ExecuteCommandList(commandList, queueIndex);
}

void DX12Device::ExecuteWaitingLists(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
{
	DX12ManagerCommandList* commandListManager = GetCommandListManager(type);
	if (commandListManager != nullptr)
		commandListManager->ExecuteWaitingList(queueIndex);
}

void DX12Device::ExecuteAllWaitingListsType(D3D12_COMMAND_LIST_TYPE type)
{
	DX12ManagerCommandList* manager = GetCommandListManager(type);
	if (manager)
		manager->ExecuteAllWaitingLists();
}

void DX12Device::ExecuteAllWaitingLists()
{
    if (m_directList)
        m_directList->ExecuteAllWaitingLists();
    if (m_computeList)
        m_computeList->ExecuteAllWaitingLists();
    if (m_copyList)
        m_copyList->ExecuteAllWaitingLists();
}

void DX12Device::SignalQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
{
	DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
    if (commandQueue)
        commandQueue->Signal();
}

void DX12Device::SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
{
	DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
    if (commandQueue)
        commandQueue->SyncQueue(INFINITE);
}

void DX12Device::SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex, UINT64 fenceValue)
{
	DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
	if (commandQueue)
		commandQueue->SyncQueue(INFINITE, fenceValue);
}

void DX12Device::ResetQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
{
    DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
    if (commandQueue)
        commandQueue->Reset();
}

void DX12Device::StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, D3D12_COMMAND_LIST_TYPE waitType, UINT waitIndex, UINT64 waitValue)
{
    DX12CommandQueue* stalledQueue = GetCommandQueue(stallType, stallIndex);
    DX12CommandQueue* toWaitQueue = GetCommandQueue(waitType, waitIndex);
	if (stalledQueue && toWaitQueue)
	{
		UINT64 valueToWait = (waitValue == 0) ? toWaitQueue->GetFenceValue() : waitValue;
		stalledQueue->StallQueue(toWaitQueue->GetFence(), valueToWait);
	}
}

void DX12Device::StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, ID3D12Fence* fence, UINT64 fenceValue)
{
	DX12CommandQueue* stalledQueue = GetCommandQueue(stallType, stallIndex);
	if (stalledQueue)
		stalledQueue->StallQueue(fence, fenceValue);
}

void DX12Device::SignalAllQueues()
{
    for (const unique_ptr<DX12CommandQueue>& queue : m_directQueue)
    {
        queue->Signal();
    }
    for (const unique_ptr<DX12CommandQueue>& queue : m_computeQueue)
    {
        queue->Signal();
    }
    for (const unique_ptr<DX12CommandQueue>& queue : m_copyQueue)
    {
        queue->Signal();
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

void DX12Device::ResetAllQueues()
{
    for (const unique_ptr<DX12CommandQueue>& queue : m_directQueue)
    {
        queue->Reset();
    }
    for (const unique_ptr<DX12CommandQueue>& queue : m_computeQueue)
    {
        queue->Reset();
    }
    for (const unique_ptr<DX12CommandQueue>& queue : m_copyQueue)
    {
        queue->Reset();
    }
}

UINT64 DX12Device::GetFenceValue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
{
	DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
	if (commandQueue)
		return commandQueue->GetFenceValue();
	return FENCE_SIGNAL_VALUE_MAX;
}

unique_ptr<DX12CommandList> DX12Device::GetCommandList(D3D12_COMMAND_LIST_TYPE type)
{
    if (type == D3D12_COMMAND_LIST_TYPE_BUNDLE)
    {
        unique_ptr<DX12CommandList> list = make_unique<DX12CommandList>();
        unique_ptr<DX12CommandAllocator> allocator = make_unique<DX12CommandAllocator>();
        allocator->Initialize(m_device.GetInterface(), type, nullptr);
        list->Initialize(m_device.GetInterface(), m_device.GetNodeMask(), type, std::move(allocator));
        return list;
    }
    else
    {
        DX12ManagerCommandList* commandListManager = GetCommandListManager(type);
        if (commandListManager)
            return commandListManager->GetCommandList();
        return nullptr;
    }
}

DX12CommandQueue* DX12Device::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
{
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		if (queueIndex < m_directQueue.size())
			return m_directQueue[queueIndex].get();
		return nullptr;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		if (queueIndex < m_computeQueue.size())
			return m_computeQueue[queueIndex].get();
		return nullptr;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		if (queueIndex < m_copyQueue.size())
			return m_copyQueue[queueIndex].get();
		return nullptr;
	default:
		return nullptr;
	}
}

DX12ManagerCommandList* DX12Device::GetCommandListManager(D3D12_COMMAND_LIST_TYPE type)
{
    switch (type)
    {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
        return m_directList.get();
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
        return m_computeList.get();
    case D3D12_COMMAND_LIST_TYPE_COPY:
        return m_copyList.get();
    default:
        return nullptr;
    }
}
