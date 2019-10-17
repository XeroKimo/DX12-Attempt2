#pragma once
#include "DX12Header.h"
#include "Level 2/CommandList.h"
#include "Level 2/CommandAllocator.h"

namespace RendererDX12
{
    class CommandQueue;
    class ManagerConstantBuffer;
    class ManagerCommandList
    {
    private:
        class WaitingList
        {
        public:
            void AddCommandList(unique_ptr<CommandList>& commandList);
            void ExecuteWaitingList(CommandQueue* commandQueue, std::vector<unique_ptr<CommandList>>& inactiveList);
        private:
            std::vector<unique_ptr<CommandList>> m_waitingList;
            std::vector<ID3D12CommandList*> m_rawList;
            std::vector<unique_ptr<CommandAllocator>> m_waitingAllocator;
        };

    public:
        ManagerCommandList( BaseDevice* device, D3D12_COMMAND_LIST_TYPE type, std::vector<unique_ptr<CommandQueue>>* commandQueues, ManagerCommandAllocator* allocatorManager, ManagerConstantBuffer* constantBufferManager);

        void CloseCommandList(unique_ptr<CommandList>& list, UINT queueIndex);
        void ExecuteCommandList(unique_ptr<CommandList>& list, UINT queueIndex);
        void ExecuteWaitingList(UINT queueIndex);
        void ExecuteAllWaitingLists();

        unique_ptr<CommandList> GetCommandList();
    private:
        BaseDevice* m_device;
        ManagerCommandAllocator* m_allocatorManager;
        ManagerConstantBuffer* m_constantBufferManager;
        std::vector<unique_ptr<CommandQueue>>* m_pCommandQueues;
        std::vector<unique_ptr<CommandList>> m_inactiveList;
        std::vector<WaitingList> m_waitingLists;
        D3D12_COMMAND_LIST_TYPE m_type;
        UINT m_nodeMask;
    };
}