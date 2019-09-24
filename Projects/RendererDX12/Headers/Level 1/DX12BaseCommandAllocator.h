#pragma once
#include "DX12Header.h"

class DX12BaseCommandAllocator
{
public:
	DX12BaseCommandAllocator();
	void Initialize(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type);

	inline ID3D12CommandAllocator* GetInterface() { return m_allocator.Get(); }
	inline D3D12_COMMAND_LIST_TYPE GetType() { return m_type; }
private:
	ComPtr<ID3D12CommandAllocator> m_allocator;
	D3D12_COMMAND_LIST_TYPE m_type;
};