#include "EventManager/EventManager.h"

namespace WinApplication
{
    void EventManager::DispatchEvents()
    {
        for (std::unique_ptr<EventDispatcherBase>& dispatcher : m_eventDispatchers)
            dispatcher->DispatchEvents();
    }
}