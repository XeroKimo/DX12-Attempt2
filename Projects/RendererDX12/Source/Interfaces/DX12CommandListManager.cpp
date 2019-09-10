#include "RendererDX12.h"
#include "Interfaces/DX12CommandListManager.h"

DX12CommandListManager::DX12CommandListManager()
{
}

void DX12CommandListManager::Initialize(DX12Device* device, DX12CommandAllocatorManager* allocatorManager, D3D12_COMMAND_LIST_TYPE type)
{
	m_device = device;
	m_allocatorManager = allocatorManager;
	m_type = type;
}

shared_ptr<DX12CommandList> DX12CommandListManager::GetCommandList()
{
	shared_ptr<DX12CommandList> list;
	if (m_inactiveLists.empty())
	{
		list = make_shared<DX12CommandList>();
		list->Initialize(m_device, this, m_allocatorManager->GetAllocator(m_type), m_type);
	}
	else
	{
		list = m_inactiveLists.back();
		m_inactiveLists.pop_back();
		list->Reset(m_allocatorManager->GetAllocator(m_type));
	}
	return list;
}

void DX12CommandListManager::ExecuteList()
{
	if (m_waitingLists.size() == 0)
		return;
	std::vector<ComPtr<ID3D12CommandList>> lists;
	lists.reserve(m_waitingLists.size());
	for (int i = 0; i < m_waitingLists.size(); i++)
	{
		lists.push_back(m_waitingLists[i]->GetCommandList());
		m_inactiveLists.push_back(m_waitingLists[i]);
	}
	m_device->ExecuteCommandLists(m_waitingLists.size(), lists[0].GetAddressOf());
	m_waitingLists.clear();

}

void DX12CommandListManager::CloseList(shared_ptr<DX12CommandList> list)
{
	m_waitingLists.push_back(list);
}
