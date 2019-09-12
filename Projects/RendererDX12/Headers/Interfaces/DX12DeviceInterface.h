#pragma once
#include "DX12Header.h"

class DX12DeviceInterface
{
public:
    DX12DeviceInterface();

    void Initialize(DX12CommandAllocatorManager* manager, D3D_FEATURE_LEVEL featureLevel, UINT adapterID, UINT directQueues = 1, UINT copyQueues = 0, UINT computeQueues = 0);

    void ExecuteCommandLists(UINT numCommandLists, ID3D12CommandList* const* commandLists, UINT queueIndex);
    void ExecuteCommandListManager(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT, UINT queueIndex = 0);
    void ExecuteAllCommandListManager();
    void SignalAllQueues();
    void SyncAllQueues();

    DX12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT, UINT index = 0);
    shared_ptr<DX12CommandList> GetCommandList(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT, UINT queuePreference = 0);
    inline DX12Device* GetDevice() { return m_device.get(); }
private:
    unique_ptr<DX12Device> m_device;

    std::vector<unique_ptr<DX12CommandQueue>> m_directQueue;
    std::vector<unique_ptr<DX12CommandQueue>> m_computeQueue;
    std::vector<unique_ptr<DX12CommandQueue>> m_copyQueue;

    unique_ptr<DX12CommandListManager> m_directList;
    unique_ptr<DX12CommandListManager> m_computeList;
    unique_ptr<DX12CommandListManager> m_copyList;
};