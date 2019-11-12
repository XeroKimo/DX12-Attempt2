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

    m_commandAllocatorManager = make_unique<ManagerCommandAllocator>(m_device.get());
    m_deviceCommandModule = make_unique<DeviceCommandModule>(m_device.get(), m_commandAllocatorManager.get(), 1, 0, 0);
    m_swapChain = make_unique<BaseSwapChain>(m_device.get(), m_deviceCommandModule->GetCommandQueueInterface(D3D12_COMMAND_LIST_TYPE_DIRECT, 0), window->GetWindowHandle(), window->GetWindowWidth(), window->GetWindowHeight());
    if (!m_swapChain->GetInterface())
        return false;

    m_moduleManager->GetModule<WinApp>()->GetWindow()->passThrough.m_mappedEvents[WM_WINDOWPOSCHANGED].Bind<Renderer,&Renderer::OnWindowSizeChange>(this);
    //RegisterListener(static_cast<IEventListenerRenderer*>(this));

    return true;
}

//void Renderer::IEventListenerRenderer::OnEvent(WinApplication::IEvent* pEvent)
//{
//}

void Renderer::OnWindowSizeChange(WPARAM wParam, LPARAM lParam)
{
    WINDOWPOS* pos = reinterpret_cast<WINDOWPOS*>(lParam);

    if (!(pos->flags & SWP_NOSIZE))
    {
        int width, height;
        m_moduleManager->GetModule<WinApp>()->GetWindow()->GetWindowSize(width, height);
        m_deviceCommandModule->SyncAllQueues();
        m_swapChain->ResizeSwapChain();
    }
}

void Renderer::OnModuleRegisterChanged(ModuleManager* moduleManager)
{
    m_moduleManager = moduleManager;
}