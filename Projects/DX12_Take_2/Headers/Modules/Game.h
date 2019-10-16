#pragma once
#include <Application.h>
#include <EventDispatcher.h>

class Game : public WinApplication::IApp, public EventManagerLib::EventDispatcher
{

public:

    // Inherited via EventDispatcher
    virtual void OnEvent(EventManagerLib::IEvent* pEvent) override;

    // Inherited via IApplication
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;
};