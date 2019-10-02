#include "RendererDX12.h"
#include "Level 5/DX12BasicInterface.h"

DX12BasicInterface::DX12BasicInterface()
{
}

bool DX12BasicInterface::Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight)
{
	m_device.Initialize(D3D_FEATURE_LEVEL_11_0, 0, &m_allocatorManager);
	m_swapChain.Initialize(m_device.GetBase()->GetInterface(), m_device.GetBase()->GetNodeMask(), m_device.GetCommandQueue()->GetBase()->GetInterface(), windowHandle, windowWidth, windowHeight);
	m_allocatorManager.Initialize(m_device.GetBase()->GetInterface(), &m_uploadBufferManager);
	m_uploadBufferManager.Initialize(m_device.GetBase()->GetInterface(), m_device.GetBase()->GetNodeMask(), m_device.GetBase()->GetNodeMask(), 256);

	return true;
}

void DX12BasicInterface::Present()
{
	SignalCommandQueue();
	SyncCommandQueue();
	m_swapChain.GetInterface()->Present(0, 0);


	ResetCommandQueue();
}
