#pragma once
#include <EventManagerLib.h>

class EventManager
{
private:
    EventManagerLib::EventManager m_manager;

public:
    bool RegisterEventDispatcher(EventManagerLib::EventDispatcher* dispatcher, std::shared_ptr<EventManagerLib::IEvent> eventKey) { return m_manager.RegisterEventDispatcher(dispatcher, eventKey); }
    bool RegisterListener(EventManagerLib::IEventListener* callbackFunc, std::shared_ptr<EventManagerLib::IEvent> eventKey) { return m_manager.RegisterListener(callbackFunc, eventKey); }
    void RecordEvent(std::shared_ptr<EventManagerLib::IEvent> pEvent) { m_manager.RecordEvent(pEvent); }
    void DispatchEvents() { m_manager.DispatchEvents(); }
};