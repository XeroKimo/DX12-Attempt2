#include "RendererDX12.h"

DX12Renderer::DX12Renderer()
{
}

bool DX12Renderer::Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight)
{
	m_allocatorManager = make_unique<DX12MCommandAllocatorManager>();
	m_device = make_unique<DX12DeviceInterface>();

	m_device->Initialize(m_allocatorManager.get(), D3D_FEATURE_LEVEL_12_0, 0);
	m_allocatorManager->Initialize(m_device->GetDevice()->GetDevice());

	m_swapChain = make_unique<DX12SwapChain>();
	m_swapChain->Initialize(m_device->GetDevice()->GetDevice(), m_device->GetCommandQueue()->GetCommandQueue(), windowHandle, windowWidth, windowHeight);

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
