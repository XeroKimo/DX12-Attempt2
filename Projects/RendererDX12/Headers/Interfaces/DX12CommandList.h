#pragma once
#include "DX12Header.h"
class DX12CommandAllocator;
class DX12CommandListManager;

class DX12CommandList : public std::enable_shared_from_this<DX12CommandList>
{
public:
	DX12CommandList();
	void Initialize(DX12Device* device, DX12CommandListManager* manager, shared_ptr<DX12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type, UINT queuePreference);

	void Reset(shared_ptr<DX12CommandAllocator> allocator, UINT queuePreference = 0);
	void LocalReset(UINT queuePreference = 0);

	void Close();
	void ExecuteAll();
	void CloseAndExecuteThis();
	inline ID3D12GraphicsCommandList1* GetCommandList() { return m_commandList.Get(); }
	inline UINT GetQueuePreference() { return m_queuePreference; }

	static void ReEnlistCommandList(shared_ptr<DX12CommandList>& commandList);
private:
	void LocalReEnlist();
private:
	DX12Device* m_device;
	DX12CommandListManager* m_manager;
	shared_ptr<DX12CommandAllocator> m_allocator;
	ComPtr<ID3D12GraphicsCommandList1> m_commandList;
	UINT m_queuePreference;
};