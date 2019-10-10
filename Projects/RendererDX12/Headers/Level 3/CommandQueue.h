#pragma once
#include "DX12Header.h"
#include "Level 2/BaseCommandQueue.h"
#include "Level 2/CommandAllocator.h"

namespace RendererDX12
{
    class ManagerCommandAllocator;
    class CommandQueue
    {
    private:
        static const size_t MAX_SIGNAL_HISTORY = 8;
    public:
        CommandQueue( BaseDevice* device, D3D12_COMMAND_LIST_TYPE commandListType, ManagerCommandAllocator* allocatorManager);

        UINT64 Signal();
        void SyncQueue(DWORD milliseconds);
        void SyncQueue(DWORD milliseconds, UINT64 fenceValue);

        void SetActiveAllocators(std::vector<unique_ptr<CommandAllocator>>& allocator);
        void Reset();
        inline void StallQueue( Fence* fence, UINT64 fenceValue) { if (fenceValue <= fence->fenceValue) m_commandQueue.StallQueue(fence, fenceValue); }
        inline void SetActiveAllocator(unique_ptr<CommandAllocator>& allocator) { m_runningAllocators.push_back(std::move(allocator)); }

        inline  Fence* GetFence() { return m_commandQueue.GetFence(); }

        inline ID3D12CommandQueue* GetInterface() { return m_commandQueue.GetInterface(); }
        inline ID3D12Fence* GetFenceInterface() { return m_commandQueue.GetFenceInterface(); }

    private:
        ManagerCommandAllocator* m_allocatorManager;
        BaseCommandQueue m_commandQueue;

        std::vector<unique_ptr<CommandAllocator>> m_runningAllocators;

        std::array<size_t, MAX_SIGNAL_HISTORY> m_allocatorSizeHistory;
        size_t m_highestSignaledSize;
    };
}