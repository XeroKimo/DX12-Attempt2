#include "RendererDX12.h"

namespace RendererDX12
{
    CommandQueue::CommandQueue(BaseDevice* device, D3D12_COMMAND_LIST_TYPE commandListType, ManagerCommandAllocator* allocatorManager, ManagerConstantBuffer* constantBufferManager) :
        m_commandQueue(device, commandListType),
        m_allocatorManager(allocatorManager),
        m_allocatorSizeHistory(),
        m_constantBufferManager(constantBufferManager)
    {
        m_allocatorSizeHistory.fill(0);
    }

    UINT64 CommandQueue::Signal()
    {
        if (m_runningAllocators.empty())
            return FENCE_MAX_SIGNAL_VALUE;
        UINT64 value = m_commandQueue.Signal();
        m_allocatorSizeHistory[(m_commandQueue.GetFence()->fenceValue - 1) % MAX_SIGNAL_HISTORY] = m_runningAllocators.size();
        return value;
    }

    void CommandQueue::SyncQueue(DWORD milliseconds)
    {
        SyncQueue(milliseconds, m_commandQueue.GetFence()->fenceValue);
    }

    void CommandQueue::SyncQueue(DWORD milliseconds, UINT64 fenceValue)
    {
        if (m_runningAllocators.empty())
            return;

        const UINT64& localFenceValue = m_commandQueue.GetFence()->fenceValue;
        const UINT64& highestSyncedFenceValue = m_commandQueue.GetFence()->highestSyncedValue;

        if (!m_commandQueue.FenceSyncable(m_commandQueue.GetFence(), fenceValue))
            return;

        if (fenceValue == FENCE_MAX_SIGNAL_VALUE || fenceValue == m_commandQueue.GetFence()->fenceValue)
        {
            m_commandQueue.SyncQueue(milliseconds, fenceValue);
            TransferUploadBuffers(m_runningAllocators.begin(), m_runningAllocators.end());
            m_allocatorManager->ResetAllocators(m_runningAllocators);
            m_runningAllocators.clear();
            Reset();
            return;
        }

        UINT64 overflow = (localFenceValue - fenceValue) / MAX_SIGNAL_HISTORY;
        UINT64 valueToSync = fenceValue + MAX_SIGNAL_HISTORY * static_cast<UINT64>(overflow);
        unsigned int roundedFenceValue = (valueToSync - 1) % MAX_SIGNAL_HISTORY;
        size_t iteratorOffset = m_allocatorSizeHistory[roundedFenceValue] - m_highestSignaledSize;

        if (iteratorOffset < 1)
        {
            iteratorOffset = m_allocatorSizeHistory[roundedFenceValue];
        }
        m_commandQueue.SyncQueue(milliseconds, valueToSync);

        std::vector<unique_ptr<CommandAllocator>> allocatorsToReset;
        auto it = m_runningAllocators.begin() + iteratorOffset;
        std::move(m_runningAllocators.begin(), it, std::back_inserter(allocatorsToReset));

        TransferUploadBuffers(allocatorsToReset.begin(), allocatorsToReset.end());

        m_allocatorManager->ResetAllocators(allocatorsToReset);
        m_runningAllocators.erase(m_runningAllocators.begin(), it);

        m_highestSignaledSize = m_allocatorSizeHistory[roundedFenceValue];
    }

    void CommandQueue::SetActiveAllocators(std::vector<unique_ptr<CommandAllocator>>& allocator)
    {
        std::move(allocator.begin(), allocator.end(), std::back_inserter(m_runningAllocators));
    }

    void CommandQueue::Reset()
    {
        m_commandQueue.Reset();
        m_highestSignaledSize = 0;
    }

    void CommandQueue::TransferUploadBuffers(std::vector<unique_ptr<CommandAllocator>>::iterator begin, std::vector<unique_ptr<CommandAllocator>>::iterator end)
    {
        auto it = begin;
        while (it != end)
        {
            m_constantBufferManager->ResetBuffers((*it)->GetConstantBuffers());
            it++;
        }
    }
}