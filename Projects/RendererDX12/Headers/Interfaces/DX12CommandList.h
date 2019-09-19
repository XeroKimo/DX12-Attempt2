#pragma once
#include "DX12Header.h"
class DX12BaseCommandAllocator;
class DX12ManagerCommandList;

class DX12CommandList : public std::enable_shared_from_this<DX12CommandList>
{
public:
	DX12CommandList();
	void Initialize(ID3D12Device* device, DX12ManagerCommandList* manager, shared_ptr<DX12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type, UINT queuePreference);

	void Reset(shared_ptr<DX12CommandAllocator> allocator, UINT queuePreference);
	void SetConstantBuffer(UINT rootParamIndex, void* data, UINT64 size);
	void SetShaderResource(UINT rootParamIndex, void* data, UINT64 size);
	void SetUnorderedAccess(UINT rootParamIndex, void* data, UINT64 size);
	void UploadData(ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data, UINT64 intermediateOffset, UINT numSubResources, UINT firstSubResource = 0);

	inline ID3D12GraphicsCommandList1* GetCommandList() { return m_commandList.Get(); }
	inline shared_ptr<DX12CommandAllocator> GetCommandAllocator() { return m_allocator; }
	inline UINT GetQueuePreference() { return m_queuePreference; }

	static void Close(shared_ptr<DX12CommandList>& commandList);
	static void CloseAndExecute(shared_ptr<DX12CommandList>& commandList);
	static void CloseAndExecuteAll(shared_ptr<DX12CommandList>& commandList);
	static void CloseExecuteReset(shared_ptr<DX12CommandList>& commandList, UINT queuePreference = 0);
private:
	static void CloseList(shared_ptr<DX12CommandList>& commandList);
private:
	DX12ManagerCommandList* m_manager;
	shared_ptr<DX12CommandAllocator> m_allocator;
	ComPtr<ID3D12GraphicsCommandList1> m_commandList;
	UINT m_queuePreference;
};
