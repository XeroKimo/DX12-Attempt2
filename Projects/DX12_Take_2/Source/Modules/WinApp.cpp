#include "PCH.h"
#include "Modules/WinApp.h"

WinApp::WinApp()
{
}

void WinApp::Initialize(HINSTANCE hInstance, WNDPROC wndFunc)
{
    using namespace WinApplication;
    TDSTR className = L"DX12Renderer";
    WNDCLASS wc = WndClassStandard(hInstance, wndFunc, className);
    CreateWindowHelper helper = CreateWindowHelper::Standard(hInstance, 1280, 720, className);
    helper.ConvertToClientSize();

    m_window.Initialize(wc, helper);
    m_application.eventManager = std::make_shared<EventManager>();

    RegisterListener(static_cast<IEventListenerApplication*>(this));
}

void WinApp::OnEvent(WinApplication::IEvent* pEvent)
{
}

void WinApp::OnModuleRegisterChanged(ModuleManager* moduleManager)
{
    m_moduleManager = moduleManager;
}

const ModuleType WinApp::GetModuleType()
{
    return ModuleType::Application;
}
