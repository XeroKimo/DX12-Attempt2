#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseSwapChain.h"
#include "Level 2/DX12ManagerUploadBuffer.h"
#include "Level 3/DX12ManagerCommandAllocator.h"
#include "Level 4/DX12Device.h"

class DX12Renderer
{
public:
	DX12Renderer();
	bool Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight);
	void Present();

	inline DX12BaseSwapChain* GetSwapChain() { return &m_swapChain; }
	inline DX12Device* GetDeviceInterface() { return &m_device; }
	inline DX12ManagerCommandAllocator* GetCommandAllocatorManager() { return &m_allocatorManager; }
private:
	DX12Device m_device;
	DX12ManagerCommandAllocator m_allocatorManager;
	DX12ManagerUploadBuffer m_uploadBufferManager;
	DX12BaseSwapChain m_swapChain;
};