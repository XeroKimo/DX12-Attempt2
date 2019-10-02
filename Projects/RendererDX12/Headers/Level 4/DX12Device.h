#pragma once
#include "DX12Header.h"

class DX12Device
{
public:
    DX12Device();

    void Initialize(D3D_FEATURE_LEVEL featureLevel, UINT adapterID, DX12ManagerCommandAllocator* manager, UINT directQueues = 1, UINT copyQueues = 0, UINT computeQueues = 0);

    void ExecuteCommandListManager(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT, UINT queueIndex = 0);
    void ExecuteAllCommandListManager();
    void SignalAllQueues();
    void SyncAllQueues();

    DX12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT, UINT index = 0);
	DX12ManagerCommandList* GetCommandListManager(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	unique_ptr<DX12CommandList> GetCommandList(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
    inline DX12BaseDevice* GetBase() { return &m_device; }
private:
    DX12BaseDevice m_device;

    std::vector<unique_ptr<DX12CommandQueue>> m_directQueue;
    std::vector<unique_ptr<DX12CommandQueue>> m_computeQueue;
    std::vector<unique_ptr<DX12CommandQueue>> m_copyQueue;

    unique_ptr<DX12ManagerCommandList> m_directList;
    unique_ptr<DX12ManagerCommandList> m_computeList;
    unique_ptr<DX12ManagerCommandList> m_copyList;
};