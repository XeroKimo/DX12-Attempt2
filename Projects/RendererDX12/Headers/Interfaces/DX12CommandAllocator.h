#pragma once
#include "DX12Header.h"
class DX12CommandAllocatorManager;
class DX12Device;

class DX12CommandAllocator : public std::enable_shared_from_this<DX12CommandAllocator>
{
public:
	DX12CommandAllocator();
	void Initialize(DX12Device* device, DX12CommandAllocatorManager* manager, const D3D12_COMMAND_LIST_TYPE& type);

	void SetActive();
	inline ComPtr<ID3D12CommandAllocator> GetAllocator() { return m_allocator; }
	inline D3D12_COMMAND_LIST_TYPE GetType() { return m_type; }
private:
	DX12CommandAllocatorManager* m_manager;
	ComPtr<ID3D12CommandAllocator> m_allocator;
	D3D12_COMMAND_LIST_TYPE m_type;
};