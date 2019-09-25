#pragma once
#include "DX12Header.h"

class DX12BaseCommandList
{
public:
	inline void Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* allocator) 
	{
		device->CreateCommandList(nodeMask, type, allocator, nullptr, IID_PPV_ARGS(m_commandList.GetAddressOf()));
	}

	inline ID3D12GraphicsCommandList* GetInterface() { return m_commandList.Get(); }
private:
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};