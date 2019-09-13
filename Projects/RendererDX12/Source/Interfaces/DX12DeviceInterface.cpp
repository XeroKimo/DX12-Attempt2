#include "RendererDX12.h"
#include "Interfaces/DX12DeviceInterface.h"

DX12DeviceInterface::DX12DeviceInterface()
{
}

void DX12DeviceInterface::Initialize(DX12MCommandAllocatorManager* manager, D3D_FEATURE_LEVEL featureLevel, UINT adapterID, UINT directQueues, UINT copyQueues, UINT computeQueues)
{
    m_device = make_unique<DX12Device>();
    m_device->Initialize(featureLevel, adapterID);

    m_directQueue.reserve(directQueues);
    m_copyQueue.reserve(copyQueues);
    m_computeQueue.reserve(computeQueues);

    if (directQueues)
    {
        m_directList = make_unique<DX12MCommandListManager>();
        m_directList->Initialize(m_device->GetDevice(),&m_directQueue, manager, D3D12_COMMAND_LIST_TYPE_DIRECT, directQueues);

        for (UINT i = 0; i < directQueues; i++)
        {
            m_directQueue.push_back(make_unique<DX12CommandQueue>());
            m_directQueue[i]->Initialize(m_device->GetDevice(), manager, D3D12_COMMAND_LIST_TYPE_DIRECT);
        }
    }

    if (computeQueues)
    {
        m_computeList = make_unique<DX12MCommandListManager>();
        m_computeList->Initialize(m_device->GetDevice(), &m_computeQueue, manager, D3D12_COMMAND_LIST_TYPE_DIRECT, computeQueues);
        for (UINT i = 0; i < copyQueues; i++)
        {
            m_computeQueue.push_back(make_unique<DX12CommandQueue>());
            m_computeQueue[i]->Initialize(m_device->GetDevice(), manager, D3D12_COMMAND_LIST_TYPE_COPY);
        }
    }
    if (copyQueues)
    {
        m_copyList = make_unique<DX12MCommandListManager>();
        m_copyList->Initialize(m_device->GetDevice(), &m_copyQueue, manager, D3D12_COMMAND_LIST_TYPE_DIRECT, copyQueues);
        for (UINT i = 0; i < computeQueues; i++)
        {
            m_copyQueue.push_back(make_unique<DX12CommandQueue>());
            m_copyQueue[i]->Initialize(m_device->GetDevice(), manager, D3D12_COMMAND_LIST_TYPE_COMPUTE);
        }
    }
}


void DX12DeviceInterface::ExecuteCommandLists(UINT numCommandLists, ID3D12CommandList* const* commandLists, UINT queueIndex)
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

void DX12DeviceInterface::ExecuteAllCommandListManager()
{
    if (m_directList)
        m_directList->ExecuteAllLists();
    if (m_computeList)
        m_computeList->ExecuteAllLists();
    if (m_copyList)
        m_copyList->ExecuteAllLists();
}

void DX12DeviceInterface::SignalAllQueues()
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


void DX12DeviceInterface::SyncAllQueues()
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


DX12CommandQueue* DX12DeviceInterface::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type, UINT index)
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

void DX12DeviceInterface::ExecuteCommandListManager(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
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

shared_ptr<DX12CommandList> DX12DeviceInterface::GetCommandList(D3D12_COMMAND_LIST_TYPE type, UINT queuePreference)
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
