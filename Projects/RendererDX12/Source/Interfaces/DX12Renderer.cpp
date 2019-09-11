#include "RendererDX12.h"

DX12Renderer::DX12Renderer()
{
}

bool DX12Renderer::Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight)
{
	m_allocatorManager = make_unique<DX12CommandAllocatorManager>();
	m_device = make_unique<DX12Device>();

	m_device->Initialize(this, m_allocatorManager.get());
	m_allocatorManager->Initialize(m_device.get());

	m_swapChain = make_unique<DX12SwapChain>();
	m_swapChain->Initialize(m_device.get(), windowHandle, windowWidth, windowHeight);

	return true;
}

void DX12Renderer::Present()
{
	m_device->ExecuteCommandListManager();
	m_device->GetCommandQueue()->SignalGPU();
	m_device->GetCommandQueue()->SyncQueue(INFINITE);

	m_swapChain->GetSwapChain()->Present(0, 0);
	m_device->GetCommandQueue()->ResetFenceValue();
}
