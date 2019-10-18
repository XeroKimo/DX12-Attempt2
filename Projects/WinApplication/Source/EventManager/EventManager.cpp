#include "EventManager/EventManager.h"

namespace WinApplication
{
    void EventManager::RecordEvent(std::unique_ptr<IEvent> pEvent)
    {
        m_eventDispatchers[m_registeredEventTypes[pEvent->GetHashKey()]]->RecordEvent(std::move(pEvent));
    }

    void EventManager::DispatchEvents()
    {
        for (std::unique_ptr<EventDispatcherBase>& dispatcher : m_eventDispatchers)
            dispatcher->DispatchEvents();
    }
}