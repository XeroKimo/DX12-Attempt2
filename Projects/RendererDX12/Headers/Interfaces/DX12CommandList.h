#pragma once
#include "DX12Header.h"
class DX12CommandAllocator;
class DX12CommandListManager;

class DX12CommandList : public std::enable_shared_from_this<DX12CommandList>
{
public:
	DX12CommandList();
	void Initialize(DX12Device* device, DX12CommandListManager* manager, shared_ptr<DX12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type, UINT queuePreference);

	void Reset(shared_ptr<DX12CommandAllocator> allocator, UINT queuePreference );

	inline ID3D12GraphicsCommandList1* GetCommandList() { return m_commandList.Get(); }
	inline shared_ptr<DX12CommandAllocator> GetCommandAllocator() { return m_allocator; }
	inline UINT GetQueuePreference() { return m_queuePreference; }

	static void Close(shared_ptr<DX12CommandList>& commandList);
	static void CloseAndExecute(shared_ptr<DX12CommandList>& commandList);
	static void CloseAndExecuteAll(shared_ptr<DX12CommandList>& commandList);
	static void CloseExecuteReset(shared_ptr<DX12CommandList>& commandList, UINT queuePreference = 0);
private:
	static void CloseList(shared_ptr<DX12CommandList> commandList);
	static void Execute(shared_ptr<DX12CommandList> commandList);
private:
	DX12Device* m_device;
	DX12CommandListManager* m_manager;
	shared_ptr<DX12CommandAllocator> m_allocator;
	ComPtr<ID3D12GraphicsCommandList1> m_commandList;
	UINT m_queuePreference;
};
