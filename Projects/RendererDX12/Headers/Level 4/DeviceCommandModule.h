#pragma once
#include "DX12Header.h"
#include "Level 3/CommandQueue.h"

namespace RendererDX12
{
    class ManagerConstantBuffer;
    class DeviceCommandModule
    {
        class CommandModule
        {
        private:
            std::vector<unique_ptr<CommandQueue>> m_commandQueues;
            unique_ptr<ManagerCommandList> m_commandListManager;

        public:
            CommandModule(const UINT& queueAmount, BaseDevice* device, D3D12_COMMAND_LIST_TYPE type, ManagerCommandAllocator* commandAllocatorManager, ManagerConstantBuffer* constantBufferManager)
            {
                m_commandQueues.reserve(static_cast<size_t>(queueAmount));
                while (m_commandQueues.size() != static_cast<size_t>(queueAmount))
                {
                    m_commandQueues.push_back(make_unique<CommandQueue>(device, type, commandAllocatorManager, constantBufferManager));
                }
                m_commandListManager = make_unique<ManagerCommandList>(device, type, &m_commandQueues, commandAllocatorManager, constantBufferManager);
            }

            CommandQueue* GetCommandQueue(UINT queueIndex)
            {
                if (queueIndex < m_commandQueues.size())
                    return m_commandQueues[queueIndex].get();
                assert(false);
                return nullptr;
            }

            ManagerCommandList* GetCommandListManager()
            {
                return m_commandListManager.get();
            }

            void SignalAllQueues()
            {
                for (const auto& commandQueue : m_commandQueues)
                    commandQueue->Signal();
            }
            void SyncAllQueues(DWORD milliseconds)
            {
                for (const auto& commandQueue : m_commandQueues)
                    commandQueue->SyncQueue(milliseconds);
            }
            void ResetAllQueues()
            {
                for (const auto& commandQueue : m_commandQueues)
                    commandQueue->Reset();
            }
        };
    public:
        DeviceCommandModule(BaseDevice* device, ManagerCommandAllocator* manager, UINT directQueues, UINT computeQueues, UINT copyQueues);
        ~DeviceCommandModule();

        void CloseCommandList(unique_ptr<CommandList>& commandList, UINT queueIndex);
        void ExecuteCommandList(unique_ptr<CommandList>& commandList, UINT queueIndex);
        void ExecuteWaitingLists(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
        void ExecuteAllWaitingListsType(D3D12_COMMAND_LIST_TYPE type);
        void ExecuteAllWaitingLists();

        UINT64 SignalQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
        void SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
        void SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex, UINT64 fenceValue);
        void ResetQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);

        void StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, D3D12_COMMAND_LIST_TYPE waitType, UINT waitIndex, UINT64 waitValue = FENCE_MAX_SIGNAL_VALUE);
        void StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex,  Fence* fence, UINT64 fenceValue = FENCE_MAX_SIGNAL_VALUE);

        void SignalAllQueues();
        void SyncAllQueues();
        void ResetAllQueues();

        UINT64 GetFenceValue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);

        unique_ptr<CommandList> GetCommandList(D3D12_COMMAND_LIST_TYPE type);
        inline ID3D12CommandQueue* GetCommandQueueInterface(D3D12_COMMAND_LIST_TYPE type, UINT index) { return GetCommandQueue(type, index)->GetInterface(); }
        inline  BaseDevice* GetDevice() { return m_device; }

    private:
        void StallQueue(CommandQueue* queueToStall,  Fence* fenceToWait, UINT64 valueToWait);
        CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
        ManagerCommandList* GetCommandListManager(D3D12_COMMAND_LIST_TYPE type);
        void CreateCommandModule(const UINT& queueAmount, D3D12_COMMAND_LIST_TYPE type, ManagerCommandAllocator* commandAllocatorManager);
    private:
        BaseDevice* m_device;

        ManagerConstantBuffer m_constantBufferManager;

        std::unordered_map<D3D12_COMMAND_LIST_TYPE, unique_ptr<CommandModule>> m_commandModules;
    };
}