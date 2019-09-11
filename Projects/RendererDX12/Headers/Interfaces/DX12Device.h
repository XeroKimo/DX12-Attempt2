#pragma once
#include "DX12Header.h"
#include "DX12CommandQueue.h"
#include "DX12CommandListManager.h"

class DX12Renderer;
class DX12Device
{
public:
	DX12Device();
	void Initialize(DX12Renderer* renderer, DX12CommandAllocatorManager* manager, UINT directQueues = 1, UINT copyQueues = 0, UINT computeQueues = 0, D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0, UINT adapterID = 0, UINT nodeMask = 0);

	void ExecuteCommandLists(UINT numCommandLists, ID3D12CommandList*const* commandLists, UINT queueIndex);
	void ExecuteCommandListManager(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	void ExecuteAllCommandListManager();
	void SignalAllQueues();
	void SyncAllQueues();
	DX12CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT, UINT index = 0);
	
	shared_ptr<DX12CommandList> GetCommandList(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT, UINT queuePreference = 0);
	inline ID3D12Device2* GetDevice() { return m_device.Get(); }
	inline UINT GetNodeMask() { return m_nodeMask; }
private:
	ComPtr<ID3D12Device2> m_device;
	ComPtr<IDXGIAdapter3> m_adapter;
	UINT m_nodeMask;

	std::vector<unique_ptr<DX12CommandQueue>> m_directQueue;
	std::vector<unique_ptr<DX12CommandQueue>> m_computeQueue;
	std::vector<unique_ptr<DX12CommandQueue>> m_copyQueue;

	unique_ptr<DX12CommandListManager> m_directList;
	unique_ptr<DX12CommandListManager> m_computeList;
	unique_ptr<DX12CommandListManager> m_copyList;
};