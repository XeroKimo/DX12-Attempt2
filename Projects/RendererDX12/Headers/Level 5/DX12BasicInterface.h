#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseSwapChain.h"
#include "Level 2/DX12ManagerUploadBuffer.h"
#include "Level 3/DX12ManagerCommandAllocator.h"
#include "Level 4/DX12Device.h"

class DX12BasicInterface
{
public:
	DX12BasicInterface();
	bool Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight);
	void Present();

	//inline void CloseCommandList(unique_ptr<DX12CommandList>& commandList) { m_device.GetCommandListManager(D3D12_COMMAND_LIST_TYPE_DIRECT)->CloseCommandList(commandList, 0); }
	//inline void ExecuteCommandList(unique_ptr<DX12CommandList>& commandList) { m_device.GetCommandListManager(D3D12_COMMAND_LIST_TYPE_DIRECT)->ExecuteList(commandList, 0); }
	//inline void ExecuteWaitingCommandLists() { m_device.GetCommandListManager(D3D12_COMMAND_LIST_TYPE_DIRECT)->ExecuteWaitingList(0); }

	//inline void SignalCommandQueue() { m_device.GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT,0)->Signal(); }
	//inline void SyncCommandQueue() { m_device.GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT,0)->SyncQueue(INFINITE); }
	//inline void ResetCommandQueue() { m_device.GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT,0)->GetBase()->ResetFenceValue(); }

	inline unique_ptr<DX12CommandList> GetCommandList() { return m_device.GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT); }
	inline DX12BaseSwapChain* GetSwapChain() { return &m_swapChain; }
	inline DX12Device* GetDeviceInterface() { return &m_device; }
	inline DX12ManagerCommandAllocator* GetCommandAllocatorManager() { return &m_allocatorManager; }
private:
	DX12Device m_device;
	DX12ManagerCommandAllocator m_allocatorManager;
	DX12ManagerUploadBuffer m_uploadBufferManager;
	DX12BaseSwapChain m_swapChain;
};