#pragma once
#include "DX12Header.h"
#include "DX12CommandList.h"

class DX12Device;
class DX12CommandAllocatorManager;
class DX12CommandListManager

{
public:
	DX12CommandListManager();
	void Initialize(DX12Device* device, DX12CommandAllocatorManager* allocatorManager, D3D12_COMMAND_LIST_TYPE type);

	shared_ptr<DX12CommandList> GetCommandList();
	void CloseList(shared_ptr<DX12CommandList> list);
	void ExecuteList();
private:
	DX12Device* m_device;
	DX12CommandAllocatorManager* m_allocatorManager;
	D3D12_COMMAND_LIST_TYPE m_type;
	std::vector<shared_ptr<DX12CommandList>> m_inactiveLists;
	std::vector<shared_ptr<DX12CommandList>> m_waitingLists;
};