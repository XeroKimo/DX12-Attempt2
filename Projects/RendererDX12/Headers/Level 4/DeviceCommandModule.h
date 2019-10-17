#pragma once
#include "DX12Header.h"
#include "Level 3/CommandQueue.h"

namespace RendererDX12
{
    class ManagerConstantBuffer;
    class DeviceCommandModule
    {
    public:
        DeviceCommandModule( BaseDevice* device, ManagerCommandAllocator* manager, ManagerConstantBuffer* constantBufferManager, UINT directQueues, UINT computeQueues, UINT copyQueues);
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
    private:
         BaseDevice* m_device;

        std::vector<unique_ptr<CommandQueue>> m_directQueue;
        std::vector<unique_ptr<CommandQueue>> m_computeQueue;
        std::vector<unique_ptr<CommandQueue>> m_copyQueue;

        unique_ptr<ManagerCommandList> m_directList;
        unique_ptr<ManagerCommandList> m_computeList;
        unique_ptr<ManagerCommandList> m_copyList;
    };
}