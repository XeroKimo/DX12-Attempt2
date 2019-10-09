#pragma once
#include "DX12Header.h"
#include "Level 2/DX12CommandList.h"
#include "Level 2/DX12CommandAllocator.h"

namespace RendererDX12
{
    class DX12CommandQueue;
    class DX12ManagerCommandList
    {
    private:
        class WaitingList
        {
        public:
            void AddCommandList(unique_ptr<DX12CommandList>& commandList);
            void ExecuteWaitingList(DX12CommandQueue* commandQueue, std::vector<unique_ptr<DX12CommandList>>& inactiveList);
        private:
            std::vector<unique_ptr<DX12CommandList>> m_waitingList;
            std::vector<ID3D12CommandList*> m_rawList;
            std::vector<unique_ptr<DX12CommandAllocator>> m_waitingAllocator;
        };

    public:
        DX12ManagerCommandList(DX12BaseDevice* device, Command_List_Type type, std::vector<unique_ptr<DX12CommandQueue>>* commandQueues, DX12ManagerCommandAllocator* allocatorManager);

        void CloseCommandList(unique_ptr<DX12CommandList>& list, UINT queueIndex);
        void ExecuteCommandList(unique_ptr<DX12CommandList>& list, UINT queueIndex);
        void ExecuteWaitingList(UINT queueIndex);
        void ExecuteAllWaitingLists();

        unique_ptr<DX12CommandList> GetCommandList();
    private:
        DX12BaseDevice* m_device;
        DX12ManagerCommandAllocator* m_allocatorManager;
        std::vector<unique_ptr<DX12CommandQueue>>* m_pCommandQueues;
        std::vector<unique_ptr<DX12CommandList>> m_inactiveList;
        std::vector<WaitingList> m_waitingLists;
        Command_List_Type m_type;
        UINT m_nodeMask;
    };
}