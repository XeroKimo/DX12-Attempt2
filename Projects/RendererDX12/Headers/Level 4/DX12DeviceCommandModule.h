#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class DX12DeviceCommandModule
    {
    public:
        DX12DeviceCommandModule(DX12BaseDevice* device, DX12ManagerCommandAllocator* manager, UINT directQueues, UINT computeQueues, UINT copyQueues);
        ~DX12DeviceCommandModule();

        void CloseCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex);
        void ExecuteCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex);
        void ExecuteWaitingLists(Command_List_Type type, UINT queueIndex);
        void ExecuteAllWaitingListsType(Command_List_Type type);
        void ExecuteAllWaitingLists();

        UINT64 SignalQueue(Command_List_Type type, UINT queueIndex);
        void SyncQueue(Command_List_Type type, UINT queueIndex);
        void SyncQueue(Command_List_Type type, UINT queueIndex, UINT64 fenceValue);
        void ResetQueue(Command_List_Type type, UINT queueIndex);

        void StallQueue(Command_List_Type stallType, UINT stallIndex, Command_List_Type waitType, UINT waitIndex, UINT64 waitValue = FENCE_MAX_SIGNAL_VALUE);
        void StallQueue(Command_List_Type stallType, UINT stallIndex, DX12Fence* fence, UINT64 fenceValue = FENCE_MAX_SIGNAL_VALUE);

        void SignalAllQueues();
        void SyncAllQueues();
        void ResetAllQueues();

        UINT64 GetFenceValue(Command_List_Type type, UINT queueIndex);

        unique_ptr<DX12CommandList> GetCommandList(Command_List_Type type);
        inline ID3D12CommandQueue* GetCommandQueueInterface(Command_List_Type type, UINT index) { return GetCommandQueue(type, index)->GetInterface(); }
        inline DX12BaseDevice* GetDevice() { return m_device; }

    private:
        void StallQueue(DX12CommandQueue* queueToStall, DX12Fence* fenceToWait, UINT64 valueToWait);
        DX12CommandQueue* GetCommandQueue(Command_List_Type type, UINT queueIndex);
        DX12ManagerCommandList* GetCommandListManager(Command_List_Type type);
    private:
        DX12BaseDevice* m_device;

        std::vector<unique_ptr<DX12CommandQueue>> m_directQueue;
        std::vector<unique_ptr<DX12CommandQueue>> m_computeQueue;
        std::vector<unique_ptr<DX12CommandQueue>> m_copyQueue;

        unique_ptr<DX12ManagerCommandList> m_directList;
        unique_ptr<DX12ManagerCommandList> m_computeList;
        unique_ptr<DX12ManagerCommandList> m_copyList;
    };
}