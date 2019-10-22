#include "PCH.h"
#include "Modules/WinApp.h"

WinApp::WinApp()
{
}

bool WinApp::Initialize(HINSTANCE hInstance, WNDPROC wndFunc)
{
    using namespace WinApplication;
    TDSTR className = L"DX12Renderer";
    WNDCLASS wc = WndClassStandard(hInstance, wndFunc, className);
    CreateWindowHelper helper = CreateWindowHelper::Standard(hInstance, 1280, 720, className);
    helper.ConvertToClientSize();

    m_window.Initialize(wc, helper);

    if (!m_window.GetWindowHandle())
        return false;

    m_application.eventManager = std::make_shared<EventManager>();
    //m_window.SetUserData((LONG_PTR)(m_application.eventManager.get()));
    return true;
}

//void WinApp::IEventListenerApplication::OnEvent(WinApplication::IEvent* pEvent)
//{
//}

void WinApp::OnModuleRegisterChanged(ModuleManager* moduleManager)
{
    m_moduleManager = moduleManager;
}

void WinApp::InitializeEventDispatchers()
{
}
