#include "RendererDX12.h"

DX12Renderer::DX12Renderer()
{
}

bool DX12Renderer::Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight)
{
	m_device = make_unique<DX12Device>();
	m_swapChain = make_unique<DX12BaseSwapChain>();
	m_allocatorManager = make_unique<DX12ManagerCommandAllocator>();
	m_uploadBufferManager = make_unique<DX12ManagerUploadBuffer>();

	m_device->Initialize(m_allocatorManager.get(), D3D_FEATURE_LEVEL_12_0, 0);
	m_swapChain->Initialize(m_device->GetDevice()->GetDevice(), m_device->GetCommandQueue()->GetBaseCommandQueue()->GetCommandQueue(), windowHandle, windowWidth, windowHeight);
	m_allocatorManager->Initialize(m_device->GetDevice()->GetDevice(), m_uploadBufferManager.get());
	m_uploadBufferManager->Initialize(m_device->GetDevice()->GetDevice(), 256);


	return true;
}

void DX12Renderer::Present()
{
	m_device->ExecuteCommandListManager();
	m_device->GetCommandQueue()->SignalGPU();
	m_device->GetCommandQueue()->SyncQueue(INFINITE);

	m_swapChain->GetSwapChain()->Present(1, 0);	

	m_device->GetCommandQueue()->GetBaseCommandQueue()->ResetFenceValue();
}
