#pragma once
#include "DX12Header.h"

class DX12Device
{
public:
    DX12Device();

    void Initialize(D3D_FEATURE_LEVEL featureLevel, UINT adapterID, DX12ManagerCommandAllocator* manager, UINT directQueues, UINT computeQueues, UINT copyQueues);

    void CloseCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex);
    void ExecuteCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex);
    void ExecuteWaitingLists(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
	void ExecuteAllWaitingListsType(D3D12_COMMAND_LIST_TYPE type);
    void ExecuteAllWaitingLists();

    void SignalQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
    void SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
	void SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex, UINT64 fenceValue);
    void ResetQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);

    void StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, D3D12_COMMAND_LIST_TYPE waitType, UINT waitIndex, UINT64 waitValue = 0);
	void StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, ID3D12Fence* fence, UINT64 fenceValue);

    void SignalAllQueues();
    void SyncAllQueues();
    void ResetAllQueues();

	UINT64 GetFenceValue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
	
	unique_ptr<DX12CommandList> GetCommandList(D3D12_COMMAND_LIST_TYPE type);
    inline ID3D12CommandQueue* GetCommandQueueInterface(D3D12_COMMAND_LIST_TYPE type, UINT index) { return GetCommandQueue(type, index)->GetInterface(); }
    inline DX12BaseDevice* GetBase() { return &m_device; }

private:
    DX12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
    DX12ManagerCommandList* GetCommandListManager(D3D12_COMMAND_LIST_TYPE type);
private:
    DX12BaseDevice m_device;

    std::vector<unique_ptr<DX12CommandQueue>> m_directQueue;
    std::vector<unique_ptr<DX12CommandQueue>> m_computeQueue;
    std::vector<unique_ptr<DX12CommandQueue>> m_copyQueue;

    unique_ptr<DX12ManagerCommandList> m_directList;
    unique_ptr<DX12ManagerCommandList> m_computeList;
    unique_ptr<DX12ManagerCommandList> m_copyList;
};