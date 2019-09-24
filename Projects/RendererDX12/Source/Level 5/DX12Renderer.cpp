#include "RendererDX12.h"
#include "Level 5/DX12Renderer.h"

DX12Renderer::DX12Renderer()
{
}

bool DX12Renderer::Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight)
{
	m_device.Initialize(D3D_FEATURE_LEVEL_12_0, 0, &m_allocatorManager);
    UINT nodemask1 = m_device.GetBase()->GetNodeMask();
    DX12BaseDevice device2;
    device2.Initialize(D3D_FEATURE_LEVEL_12_0, 1);
    UINT nodemasknew1 = m_device.GetBase()->GetNodeMask();
    UINT nodemask2 = device2.GetNodeMask();
	m_swapChain.Initialize(m_device.GetBase()->GetInterface(), m_device.GetCommandQueue()->GetBase()->GetInterface(), windowHandle, windowWidth, windowHeight);
	m_allocatorManager.Initialize(m_device.GetBase()->GetInterface(), &m_uploadBufferManager);
	m_uploadBufferManager.Initialize(m_device.GetBase()->GetInterface(), 256);

	return true;
}

void DX12Renderer::Present()
{
	m_device.ExecuteCommandListManager();
	m_device.GetCommandQueue()->Signal();
	m_device.GetCommandQueue()->SyncQueue(INFINITE);

	m_swapChain.GetInterface()->Present(0, 0);

	m_device.GetCommandQueue()->GetBase()->ResetFenceValue();
}
