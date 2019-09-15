#pragma once
#include "DX12Header.h"
class DX12ManagerUploadBuffer;

class DX12Renderer
{
public:
	DX12Renderer();
	bool Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight);
	void Present();

	inline DX12BaseSwapChain* GetSwapChain() { return m_swapChain.get(); }
	inline DX12Device* GetDeviceInterface() { return m_device.get(); }
	inline DX12ManagerCommandAllocator* GetCommandAllocatorManager() { return m_allocatorManager.get(); }
private:
	unique_ptr<DX12Device> m_device;
	unique_ptr<DX12ManagerCommandAllocator> m_allocatorManager;
	unique_ptr<DX12ManagerUploadBuffer> m_uploadBufferManager;
	unique_ptr<DX12BaseSwapChain> m_swapChain;
};