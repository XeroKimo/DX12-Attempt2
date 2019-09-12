#pragma once
#include "DX12Header.h"
class DX12CommandAllocatorManager;
class DX12Device;

class DX12CommandAllocator : public std::enable_shared_from_this<DX12CommandAllocator>
{
public:
	DX12CommandAllocator();
	void Initialize(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type);

	inline ComPtr<ID3D12CommandAllocator> GetAllocator() { return m_allocator; }
	inline D3D12_COMMAND_LIST_TYPE GetType() { return m_type; }
private:
	ComPtr<ID3D12CommandAllocator> m_allocator;
	D3D12_COMMAND_LIST_TYPE m_type;
};