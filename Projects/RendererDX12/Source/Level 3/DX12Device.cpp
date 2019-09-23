#include "RendererDX12.h"
#include "Level 3/DX12Device.h"

DX12Device::DX12Device()
{
}

void DX12Device::Initialize(D3D_FEATURE_LEVEL featureLevel, UINT adapterID, DX12ManagerCommandAllocator* manager, UINT directQueues, UINT copyQueues, UINT computeQueues)
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
            m_directQueue[i]->Initialize(m_device.GetInterface(), D3D12_COMMAND_LIST_TYPE_DIRECT, manager);
        }
		m_directList = make_unique<DX12ManagerCommandList>();
		m_directList->Initialize(m_device.GetInterface(), D3D12_COMMAND_LIST_TYPE_DIRECT, &m_directQueue, manager);
    }

    if (computeQueues)
    {
        for (UINT i = 0; i < computeQueues; i++)
        {
            m_computeQueue.push_back(make_unique<DX12CommandQueue>());
            m_computeQueue[i]->Initialize(m_device.GetInterface(), D3D12_COMMAND_LIST_TYPE_COMPUTE, manager);
        }
		m_computeList = make_unique<DX12ManagerCommandList>();
		m_computeList->Initialize(m_device.GetInterface(), D3D12_COMMAND_LIST_TYPE_COMPUTE, &m_computeQueue, manager);
    }
    if (copyQueues)
    {
        for (UINT i = 0; i < copyQueues; i++)
        {
            m_copyQueue.push_back(make_unique<DX12CommandQueue>());
            m_copyQueue[i]->Initialize(m_device.GetInterface(), D3D12_COMMAND_LIST_TYPE_COPY, manager);
        }
		m_copyList = make_unique<DX12ManagerCommandList>();
		m_copyList->Initialize(m_device.GetInterface(), D3D12_COMMAND_LIST_TYPE_COPY, &m_copyQueue, manager);
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
