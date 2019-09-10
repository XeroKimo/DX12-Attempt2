#pragma once
#include "DX12Header.h"

class DX12Renderer
{
public:
	DX12Renderer();
	bool Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight);
	void Present();

	inline DX12SwapChain* GetSwapChain() { return m_swapChain.get(); }
	inline DX12Device* GetDevice() { return m_device.get(); }
	inline DX12CommandAllocatorManager* GetCommandAllocatorManager() { return m_allocatorManager.get(); }
private:
	unique_ptr<DX12Device> m_device;
	unique_ptr<DX12CommandAllocatorManager> m_allocatorManager;
	unique_ptr<DX12SwapChain> m_swapChain;
};