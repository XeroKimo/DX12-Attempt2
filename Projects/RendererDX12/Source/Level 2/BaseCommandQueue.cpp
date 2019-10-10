#include "RendererDX12.h"

namespace RendererDX12
{
    BaseCommandQueue::BaseCommandQueue(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE commandListType) :
        m_fence(device)
    {
        HRESULT hr;

        D3D12_COMMAND_QUEUE_DESC cqDesc;
        cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        cqDesc.NodeMask = nodeMask;
        cqDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        cqDesc.Type = static_cast<D3D12_COMMAND_LIST_TYPE>(commandListType);

        m_type = commandListType;

        hr = device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(m_commandQueue.GetAddressOf()));
        assert(SUCCEEDED(hr));

        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    }

    UINT64 BaseCommandQueue::Signal()
    {
        return Signal(&m_fence);
    }

    UINT64 BaseCommandQueue::Signal(Fence* fence)
    {
        fence->fenceValue++;
        m_commandQueue->Signal(fence->GetInterface(), fence->fenceValue);
        return m_fence.fenceValue;
    }

    void BaseCommandQueue::StallQueue(Fence* fence, UINT64 fenceValue)
    {
        m_commandQueue->Wait(fence->GetInterface(), fenceValue);
    }

    void BaseCommandQueue::SyncQueue(DWORD milliseconds, UINT64 fenceValue)
    {
        SyncFence(milliseconds, &m_fence, fenceValue);
    }

    void BaseCommandQueue::SyncFence(DWORD milliseconds, Fence* fence, UINT64 fenceValue)
    {
        if (fence->GetInterface()->GetCompletedValue() < fenceValue)
        {
            fence->GetInterface()->SetEventOnCompletion(fenceValue, m_fenceEvent);
            WaitForSingleObject(m_fenceEvent, milliseconds);
        }
        fence->highestSyncedValue = fenceValue;
    }
}