#pragma once
#include <Application.h>
#include <EventDispatcher.h>

class WinApp : public EventManagerLib::EventDispatcher
{
private:
    WinApplication::Application m_application;

public:
    void Run(WinApplication::IApp* app, WinApplication::Window* window) { m_application.Run(app, window); }

    // Inherited via EventDispatcher
    virtual void OnEvent(EventManagerLib::IEvent* pEvent) override;
};