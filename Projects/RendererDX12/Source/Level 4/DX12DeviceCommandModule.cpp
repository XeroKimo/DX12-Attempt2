#include "RendererDX12.h"
#include "Level 4/DX12DeviceCommandModule.h"

namespace RendererDX12
{
    DX12DeviceCommandModule::DX12DeviceCommandModule(DX12BaseDevice* device, DX12ManagerCommandAllocator* manager, UINT directQueues, UINT computeQueues, UINT copyQueues)
    {
        m_device = device;

        m_directQueue.reserve(directQueues);
        m_copyQueue.reserve(copyQueues);
        m_computeQueue.reserve(computeQueues);

        if (directQueues)
        {
            for (UINT i = 0; i < directQueues; i++)
                m_directQueue.push_back(make_unique<DX12CommandQueue>(m_device, Command_List_Type::Direct, manager));
            m_directList = make_unique<DX12ManagerCommandList>(m_device, Command_List_Type::Direct, &m_directQueue, manager);
        }

        if (computeQueues)
        {
            for (UINT i = 0; i < computeQueues; i++)
                m_computeQueue.push_back(make_unique<DX12CommandQueue>(m_device, Command_List_Type::Compute, manager));
            m_computeList = make_unique<DX12ManagerCommandList>(m_device, Command_List_Type::Compute, &m_computeQueue, manager);
        }
        if (copyQueues)
        {
            for (UINT i = 0; i < copyQueues; i++)
                m_copyQueue.push_back(make_unique<DX12CommandQueue>(m_device, Command_List_Type::Copy, manager));
            m_copyList = make_unique<DX12ManagerCommandList>(m_device, Command_List_Type::Copy, &m_copyQueue, manager);
        }
    }

    DX12DeviceCommandModule::~DX12DeviceCommandModule()
    {
        SignalAllQueues();
        SyncAllQueues();
    }

    void DX12DeviceCommandModule::CloseCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex)
    {
        DX12ManagerCommandList* commandListManager = GetCommandListManager(static_cast<Command_List_Type>(commandList->GetInterface()->GetType()));
        if (commandListManager != nullptr)
            commandListManager->CloseCommandList(commandList, queueIndex);
    }

    void DX12DeviceCommandModule::ExecuteCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex)
    {
        DX12ManagerCommandList* commandListManager = GetCommandListManager(static_cast<Command_List_Type>(commandList->GetInterface()->GetType()));
        if (commandListManager != nullptr)
            commandListManager->ExecuteCommandList(commandList, queueIndex);
    }

    void DX12DeviceCommandModule::ExecuteWaitingLists(Command_List_Type type, UINT queueIndex)
    {
        DX12ManagerCommandList* commandListManager = GetCommandListManager(type);
        if (commandListManager != nullptr)
            commandListManager->ExecuteWaitingList(queueIndex);
    }

    void DX12DeviceCommandModule::ExecuteAllWaitingListsType(Command_List_Type type)
    {
        DX12ManagerCommandList* manager = GetCommandListManager(type);
        if (manager)
            manager->ExecuteAllWaitingLists();
    }

    void DX12DeviceCommandModule::ExecuteAllWaitingLists()
    {
        if (m_directList)
            m_directList->ExecuteAllWaitingLists();
        if (m_computeList)
            m_computeList->ExecuteAllWaitingLists();
        if (m_copyList)
            m_copyList->ExecuteAllWaitingLists();
    }

    UINT64 DX12DeviceCommandModule::SignalQueue(Command_List_Type type, UINT queueIndex)
    {
        DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            return commandQueue->Signal();
        return FENCE_MAX_SIGNAL_VALUE;
    }

    void DX12DeviceCommandModule::SyncQueue(Command_List_Type type, UINT queueIndex)
    {
        DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            commandQueue->SyncQueue(INFINITE);
    }

    void DX12DeviceCommandModule::SyncQueue(Command_List_Type type, UINT queueIndex, UINT64 fenceValue)
    {
        DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            commandQueue->SyncQueue(INFINITE, fenceValue);
    }

    void DX12DeviceCommandModule::ResetQueue(Command_List_Type type, UINT queueIndex)
    {
        DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            commandQueue->Reset();
    }

    void DX12DeviceCommandModule::StallQueue(Command_List_Type stallType, UINT stallIndex, Command_List_Type waitType, UINT waitIndex, UINT64 waitValue)
    {
        DX12CommandQueue* stalledQueue = GetCommandQueue(stallType, stallIndex);
        DX12CommandQueue* toWaitQueue = GetCommandQueue(waitType, waitIndex);
        if (stalledQueue && toWaitQueue)
            StallQueue(stalledQueue, toWaitQueue->GetFence(), waitValue);
    }

    void DX12DeviceCommandModule::StallQueue(Command_List_Type stallType, UINT stallIndex, DX12Fence* fence, UINT64 fenceValue)
    {
        DX12CommandQueue* stalledQueue = GetCommandQueue(stallType, stallIndex);
        if (stalledQueue)
            StallQueue(stalledQueue, fence, fenceValue);
    }

    void DX12DeviceCommandModule::SignalAllQueues()
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

    void DX12DeviceCommandModule::SyncAllQueues()
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

    void DX12DeviceCommandModule::ResetAllQueues()
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

    UINT64 DX12DeviceCommandModule::GetFenceValue(Command_List_Type type, UINT queueIndex)
    {
        DX12CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            return commandQueue->GetFence()->fenceValue;
        return FENCE_MAX_SIGNAL_VALUE;
    }

    unique_ptr<DX12CommandList> DX12DeviceCommandModule::GetCommandList(Command_List_Type type)
    {
        if (type == Command_List_Type::Bundle)
        {
            unique_ptr<DX12CommandAllocator> allocator = make_unique<DX12CommandAllocator>(m_device->GetInterface(), type, nullptr);
            return make_unique<DX12CommandList>(m_device->GetInterface(), m_device->GetNodeMask(), type, std::move(allocator));
        }
        else
        {
            DX12ManagerCommandList* commandListManager = GetCommandListManager(type);
            if (commandListManager)
                return commandListManager->GetCommandList();
            return nullptr;
        }
    }


    void DX12DeviceCommandModule::StallQueue(DX12CommandQueue* queueToStall, DX12Fence* fenceToWait, UINT64 valueToWait)
    {
        UINT64 waitValue = (valueToWait == FENCE_MAX_SIGNAL_VALUE) ? fenceToWait->fenceValue : valueToWait;
        queueToStall->StallQueue(fenceToWait, waitValue);
    }

    DX12CommandQueue* DX12DeviceCommandModule::GetCommandQueue(Command_List_Type type, UINT queueIndex)
    {
        switch (type)
        {
        case Command_List_Type::Direct:
            if (queueIndex < m_directQueue.size())
                return m_directQueue[queueIndex].get();
            return nullptr;
        case Command_List_Type::Compute:
            if (queueIndex < m_computeQueue.size())
                return m_computeQueue[queueIndex].get();
            return nullptr;
        case Command_List_Type::Copy:
            if (queueIndex < m_copyQueue.size())
                return m_copyQueue[queueIndex].get();
            return nullptr;
        default:
            return nullptr;
        }
    }

    DX12ManagerCommandList* DX12DeviceCommandModule::GetCommandListManager(Command_List_Type type)
    {
        switch (type)
        {
        case Command_List_Type::Direct:
            return m_directList.get();
        case Command_List_Type::Compute:
            return m_computeList.get();
        case Command_List_Type::Copy:
            return m_copyList.get();
        default:
            return nullptr;
        }
    }
}