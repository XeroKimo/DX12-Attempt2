#pragma once
#include "DX12Header.h"
class DX12CommandAllocator;
class DX12CommandListManager;

class DX12CommandList : public std::enable_shared_from_this<DX12CommandList>
{
public:
	DX12CommandList();
	void Initialize(DX12Device* device, DX12CommandListManager* manager, shared_ptr<DX12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type);

	void Reset(shared_ptr<DX12CommandAllocator> allocator);
	void Close();
	void Execute();
	inline ID3D12GraphicsCommandList1* GetCommandList() { return m_commandList.Get(); }
private:
	DX12CommandListManager* m_manager;
	shared_ptr<DX12CommandAllocator> m_allocator;
	ComPtr<ID3D12GraphicsCommandList1> m_commandList;
};