#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseCommandList.h"

class DX12CommandList : public std::enable_shared_from_this<DX12CommandList>
{
public:
	DX12CommandList();
	void Initialize(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, shared_ptr<DX12CommandAllocator> allocator);
	void Reset(shared_ptr<DX12CommandAllocator> allocator);

	void SetConstantBuffer(UINT rootParamIndex, void* data, UINT64 size);
	void SetShaderResource(UINT rootParamIndex, void* data, UINT64 size);
	void SetUnorderedAccess(UINT rootParamIndex, void* data, UINT64 size);
	void UploadData(ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data, UINT64 intermediateOffset, UINT numSubResources, UINT firstSubResource = 0);

	inline void Close() { m_commandList.GetInterface()->Close(); }
	inline DX12BaseCommandList* GetBase() { return &m_commandList; }
	inline shared_ptr<DX12CommandAllocator> GetCommandAllocator() { return m_allocator; }
private:
	shared_ptr<DX12CommandAllocator> m_allocator;
	DX12BaseCommandList m_commandList;
};
