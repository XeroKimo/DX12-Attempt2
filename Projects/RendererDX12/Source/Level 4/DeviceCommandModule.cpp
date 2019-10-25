#include "RendererDX12.h"

namespace RendererDX12
{
    DeviceCommandModule::DeviceCommandModule(BaseDevice* device, ManagerCommandAllocator* manager, UINT directQueues, UINT computeQueues, UINT copyQueues) :
        m_constantBufferManager(device, (1 << 16) - 1)
    {
        m_device = device;
        if (directQueues == 0)
            directQueues = 1;

        CreateCommandModule(directQueues, D3D12_COMMAND_LIST_TYPE_DIRECT, manager);
        CreateCommandModule(computeQueues, D3D12_COMMAND_LIST_TYPE_COMPUTE, manager);
        CreateCommandModule(copyQueues, D3D12_COMMAND_LIST_TYPE_COPY, manager);
    }

    DeviceCommandModule::~DeviceCommandModule()
    {
        SignalAllQueues();
        SyncAllQueues();
    }

    void DeviceCommandModule::CloseCommandList(unique_ptr<CommandList>& commandList, UINT queueIndex)
    {
        ManagerCommandList* commandListManager = GetCommandListManager(static_cast<D3D12_COMMAND_LIST_TYPE>(commandList->GetInterface()->GetType()));
        if (commandListManager != nullptr)
            commandListManager->CloseCommandList(commandList, queueIndex);
    }

    void DeviceCommandModule::ExecuteCommandList(unique_ptr<CommandList>& commandList, UINT queueIndex)
    {
        ManagerCommandList* commandListManager = GetCommandListManager(static_cast<D3D12_COMMAND_LIST_TYPE>(commandList->GetInterface()->GetType()));
        if (commandListManager != nullptr)
            commandListManager->ExecuteCommandList(commandList, queueIndex);
    }

    void DeviceCommandModule::ExecuteWaitingLists(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
    {
        ManagerCommandList* commandListManager = GetCommandListManager(type);
        if (commandListManager != nullptr)
            commandListManager->ExecuteWaitingList(queueIndex);
    }

    void DeviceCommandModule::ExecuteAllWaitingListsType(D3D12_COMMAND_LIST_TYPE type)
    {
        ManagerCommandList* manager = GetCommandListManager(type);
        if (manager)
            manager->ExecuteAllWaitingLists();
    }

    void DeviceCommandModule::ExecuteAllWaitingLists()
    {
        for (auto& commandModule : m_commandModules)
            commandModule.second->GetCommandListManager()->ExecuteAllWaitingLists();
    }

    UINT64 DeviceCommandModule::SignalQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
    {
        CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            return commandQueue->Signal();
        return FENCE_MAX_SIGNAL_VALUE;
    }

    void DeviceCommandModule::SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
    {
        CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            commandQueue->SyncQueue(INFINITE);
    }

    void DeviceCommandModule::SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex, UINT64 fenceValue)
    {
        CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            commandQueue->SyncQueue(INFINITE, fenceValue);
    }

    void DeviceCommandModule::ResetQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
    {
        CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            commandQueue->Reset();
    }

    void DeviceCommandModule::StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, D3D12_COMMAND_LIST_TYPE waitType, UINT waitIndex, UINT64 waitValue)
    {
        CommandQueue* stalledQueue = GetCommandQueue(stallType, stallIndex);
        CommandQueue* toWaitQueue = GetCommandQueue(waitType, waitIndex);
        if (stalledQueue && toWaitQueue)
        {
            if (stalledQueue != toWaitQueue)
                StallQueue(stalledQueue, toWaitQueue->GetFence(), waitValue);
        }
    }

    void DeviceCommandModule::StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, Fence* fence, UINT64 fenceValue)
    {
        CommandQueue* stalledQueue = GetCommandQueue(stallType, stallIndex);
        if (stalledQueue)
            StallQueue(stalledQueue, fence, fenceValue);
    }

    void DeviceCommandModule::SignalAllQueues()
    {
        for (const auto& commandModule : m_commandModules)
            commandModule.second->SignalAllQueues();
    }

    void DeviceCommandModule::SyncAllQueues()
    {
        for (const auto& commandModule : m_commandModules)
            commandModule.second->SyncAllQueues(INFINITE);
    }

    void DeviceCommandModule::ResetAllQueues()
    {
        for (const auto& commandModule : m_commandModules)
            commandModule.second->ResetAllQueues();
    }

    UINT64 DeviceCommandModule::GetFenceValue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
    {
        CommandQueue* commandQueue = GetCommandQueue(type, queueIndex);
        if (commandQueue)
            return commandQueue->GetFence()->fenceValue;
        return FENCE_MAX_SIGNAL_VALUE;
    }

    unique_ptr<CommandList> DeviceCommandModule::GetCommandList(D3D12_COMMAND_LIST_TYPE type)
    {
        if (type == D3D12_COMMAND_LIST_TYPE_BUNDLE)
        {
            unique_ptr<CommandAllocator> allocator = make_unique<CommandAllocator>(m_device, type);
            return make_unique<CommandList>(m_device, std::move(allocator), nullptr);
        }
        else
        {
            ManagerCommandList* commandListManager = GetCommandListManager(type);
            if (commandListManager)
                return commandListManager->GetCommandList();
            return nullptr;
        }
    }


    void DeviceCommandModule::StallQueue(CommandQueue* queueToStall, Fence* fenceToWait, UINT64 valueToWait)
    {
        UINT64 waitValue = (valueToWait == FENCE_MAX_SIGNAL_VALUE) ? fenceToWait->fenceValue : valueToWait;
        queueToStall->StallQueue(fenceToWait, waitValue);
    }

    CommandQueue* DeviceCommandModule::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex)
    {
        if (m_commandModules.find(type) != m_commandModules.end())
            return m_commandModules[type]->GetCommandQueue(queueIndex);
        assert(false);
        return nullptr;
    }

    ManagerCommandList* DeviceCommandModule::GetCommandListManager(D3D12_COMMAND_LIST_TYPE type)
    {
        if (m_commandModules.find(type) != m_commandModules.end())
            return m_commandModules[type]->GetCommandListManager();

        assert(false);
        return nullptr;
    }

    void DeviceCommandModule::CreateCommandModule(const UINT& queueAmount, D3D12_COMMAND_LIST_TYPE type, ManagerCommandAllocator* commandAllocatorManager)
    {
        if (queueAmount == 0)
            return;

        m_commandModules[type] = make_unique<CommandModule>(queueAmount, m_device, type, commandAllocatorManager, &m_constantBufferManager);
    }
}