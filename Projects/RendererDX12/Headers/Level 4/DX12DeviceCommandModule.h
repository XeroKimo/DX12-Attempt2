#pragma once
#include "DX12Header.h"

class DX12DeviceCommandModule
{
public:
    DX12DeviceCommandModule(DX12BaseDevice* device, DX12ManagerCommandAllocator* manager, UINT directQueues, UINT computeQueues, UINT copyQueues);
	~DX12DeviceCommandModule();

    void CloseCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex);
    void ExecuteCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex);
    void ExecuteWaitingLists(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
	void ExecuteAllWaitingListsType(D3D12_COMMAND_LIST_TYPE type);
    void ExecuteAllWaitingLists();

    UINT64 SignalQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
    void SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
	void SyncQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex, UINT64 fenceValue);
    void ResetQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);

    void StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, D3D12_COMMAND_LIST_TYPE waitType, UINT waitIndex, UINT64 waitValue = FENCE_SIGNAL_VALUE_MAX);
	void StallQueue(D3D12_COMMAND_LIST_TYPE stallType, UINT stallIndex, DX12Fence* fence, UINT64 fenceValue = FENCE_SIGNAL_VALUE_MAX);

    void SignalAllQueues();
    void SyncAllQueues();
    void ResetAllQueues();

	UINT64 GetFenceValue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
	
	unique_ptr<DX12CommandList> GetCommandList(D3D12_COMMAND_LIST_TYPE type);
    inline ID3D12CommandQueue* GetCommandQueueInterface(D3D12_COMMAND_LIST_TYPE type, UINT index) { return GetCommandQueue(type, index)->GetInterface(); }
    inline DX12BaseDevice* GetDevice() { return m_device; }

private:
	void StallQueue(DX12CommandQueue* queueToStall, DX12Fence* fenceToWait, UINT64 valueToWait);
    DX12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type, UINT queueIndex);
    DX12ManagerCommandList* GetCommandListManager(D3D12_COMMAND_LIST_TYPE type);
private:
    DX12BaseDevice* m_device;

    std::vector<unique_ptr<DX12CommandQueue>> m_directQueue;
    std::vector<unique_ptr<DX12CommandQueue>> m_computeQueue;
    std::vector<unique_ptr<DX12CommandQueue>> m_copyQueue;

    unique_ptr<DX12ManagerCommandList> m_directList;
    unique_ptr<DX12ManagerCommandList> m_computeList;
    unique_ptr<DX12ManagerCommandList> m_copyList;
};