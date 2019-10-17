#pragma once
#include <WinApplication.h>
#include "ModuleManager.h"

__interface IEventListenerApplication : public WinApplication::IEventListener
{
    void OnEvent(WinApplication::IEvent* pEvent);
};

class WinApp : public WinApplication::EventDispatcher, public IModule, public IEventListenerApplication
{
private:
    ModuleManager* m_moduleManager = nullptr;

    WinApplication::Window m_window = { };
    WinApplication::Application m_application = { };

public:
    WinApp();
    void Initialize(HINSTANCE hInstance, WNDPROC wndFunc);
    void Run(WinApplication::IApp* app) { m_application.Run(app, &m_window); }

    WinApplication::Application* GetApplication() { return &m_application; }
    WinApplication::Window* GetWindow() { return &m_window; }

    // Inherited via IModule
    virtual void OnModuleRegisterChanged(ModuleManager* moduleManager) override;
    virtual const ModuleType GetModuleType() override;

    // Inherited via IEventListenerApplication
    virtual void IEventListenerApplication::OnEvent(WinApplication::IEvent* pEvent) override;
};