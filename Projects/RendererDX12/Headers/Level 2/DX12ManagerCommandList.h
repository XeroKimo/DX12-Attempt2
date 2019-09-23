#pragma once
#include "DX12Header.h"
#include "DX12CommandList.h"

class DX12ManagerCommandAllocator;
class DX12ManagerCommandList

{
public:
	DX12ManagerCommandList();
	void Initialize(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, std::vector<unique_ptr<DX12CommandQueue>>* commandQueues, DX12ManagerCommandAllocator* allocatorManager);

	shared_ptr<DX12CommandList> GetCommandList(UINT queuePreference);
	shared_ptr<DX12CommandAllocator> RequestNewAllocator();

	void CloseList(shared_ptr<DX12CommandList> list);
	void ExecuteList(UINT queueIndex);
	void ExecuteAllLists();
    void ExecuteDirect(shared_ptr<DX12CommandList> commandList);
	void ReEnlistInactiveCommandList(shared_ptr<DX12CommandList>& commandList);
private:
    ID3D12Device* m_device;
	DX12ManagerCommandAllocator* m_allocatorManager;
    std::vector<unique_ptr<DX12CommandQueue>>* m_pCommandQueues;
	std::vector<shared_ptr<DX12CommandList>> m_inactiveLists;
	std::vector<std::vector<shared_ptr<DX12CommandList>>> m_waitingLists;
	std::vector<std::vector<shared_ptr<DX12CommandAllocator>>> m_waitingAllocators;
	D3D12_COMMAND_LIST_TYPE m_type;
};