#pragma once
#include "DX12Header.h"
#include "Level 2/DX12CommandList.h"
#include "Level 2/DX12CommandAllocator.h"

class DX12CommandQueue;
class DX12ManagerCommandList
{
private:
	class WaitingList
	{
	public:
		void AddCommandList(const shared_ptr<DX12CommandList>& commandList);
		void ExecuteWaitingList(DX12CommandQueue* commandQueue, std::vector <shared_ptr<DX12CommandList>>& inactiveList);
	private:
		std::vector<shared_ptr<DX12CommandList>> m_waitingList;
		std::vector<ID3D12CommandList*> m_rawList;
		std::vector<shared_ptr<DX12CommandAllocator>> m_waitingAllocator;
	};

public:
	DX12ManagerCommandList();
	void Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, std::vector<unique_ptr<DX12CommandQueue>>* commandQueues, DX12ManagerCommandAllocator* allocatorManager);

	void CloseList(shared_ptr<DX12CommandList>& list, UINT queueIndex);
	void ExecuteList(shared_ptr<DX12CommandList>& list, UINT queueIndex);
	void ExecuteWaitingList(UINT queueIndex);
	void ExecuteAllWaitingList();

	shared_ptr<DX12CommandList> GetCommandList();
private:
    ID3D12Device* m_device;
	DX12ManagerCommandAllocator* m_allocatorManager;
    std::vector<unique_ptr<DX12CommandQueue>>* m_pCommandQueues;
	std::vector<shared_ptr<DX12CommandList>> m_inactiveList;
	std::vector<WaitingList> m_waitingLists;
	D3D12_COMMAND_LIST_TYPE m_type;
	UINT m_nodeMask;
};