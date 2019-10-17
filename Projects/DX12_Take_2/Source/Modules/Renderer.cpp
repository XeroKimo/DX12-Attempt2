#include "PCH.h"
#include "Modules/Renderer.h"

Renderer::Renderer()
{
}

bool Renderer::Initialize(WinApplication::Window* window)
{
    using namespace RendererDX12;
    m_device = make_unique<BaseDevice>(D3D_FEATURE_LEVEL_11_0, 0);
    if (!m_device->GetInterface())
        return false;

    m_constantBufferManager = make_unique<ManagerConstantBuffer>(m_device.get(), 1000);
    m_commandAllocatorManager = make_unique<ManagerCommandAllocator>(m_device.get());
    m_deviceCommandModule = make_unique<DeviceCommandModule>(m_device.get(), m_commandAllocatorManager.get(), m_constantBufferManager.get(), 1, 0, 0);
    m_swapChain = make_unique<BaseSwapChain>(m_device->GetInterface(), m_device->GetNodeMask(), m_deviceCommandModule->GetCommandQueueInterface(D3D12_COMMAND_LIST_TYPE_DIRECT, 0), window->GetWindowHandle(), window->GetWindowWidth(), window->GetWindowHeight());
    if (!m_swapChain->GetInterface())
        return false;

    return true;
}

void Renderer::OnEvent(WinApplication::IEvent* pEvent)
{
}

void Renderer::OnModuleRegisterChanged(ModuleManager* moduleManager)
{
    m_moduleManager = moduleManager;
}
