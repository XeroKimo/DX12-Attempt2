#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseCommandList.h"

class DX12CommandList
{
public:
	DX12CommandList();
	~DX12CommandList();
	void Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, unique_ptr<DX12CommandAllocator> allocator);
	void Reset(unique_ptr<DX12CommandAllocator> allocator);

	void SetConstantBuffer(UINT rootParamIndex, void* data, UINT64 size);
	void UploadData(ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data);

	inline void Close() { m_commandList.GetInterface()->Close(); }
	inline ID3D12GraphicsCommandList* GetInterface() { return m_commandList.GetInterface(); }
	inline DX12BaseCommandList* GetBase() { return &m_commandList; }
    inline D3D12_COMMAND_LIST_TYPE GetType() { return m_commandList.GetInterface()->GetType(); }
	inline unique_ptr<DX12CommandAllocator>& GetCommandAllocator() { return m_allocator; }
private:
	unique_ptr<DX12CommandAllocator> m_allocator;
	DX12BaseCommandList m_commandList;
	UINT m_nodeMask;
};
