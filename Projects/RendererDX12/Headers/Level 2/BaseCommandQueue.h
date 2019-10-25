#pragma once
#include "DX12Header.h"
#include "Level 1/Fence.h"

namespace RendererDX12
{
    class BaseDevice;
    class BaseCommandQueue
    {
    public:
        BaseCommandQueue(BaseDevice* device, D3D12_COMMAND_LIST_TYPE commandListType);

        UINT64 Signal();
        UINT64 Signal(Fence* fence);
        void StallQueue(Fence* fence, UINT64 fenceValue);
        void SyncQueue(DWORD milliseconds, UINT64 fenceValue);
        void SyncFence(DWORD milliseconds, Fence* fence, UINT64 fenceValue);

        inline bool FenceSyncable(Fence* fence, UINT64 fenceValue) { return (fenceValue < m_fence.fenceValue || fenceValue > m_fence.highestSyncedValue); }
        inline void Reset() { m_fence.Reset(); }
        inline ID3D12CommandQueue* GetInterface() { return m_commandQueue.Get(); }
        inline Fence* GetFence() { return &m_fence; }
        inline ID3D12Fence* GetFenceInterface() { return m_fence.GetInterface(); }
    private:
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        Fence m_fence;
        D3D12_COMMAND_LIST_TYPE m_type;
        HANDLE m_fenceEvent;
    };
}