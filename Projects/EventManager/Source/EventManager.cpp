#include "EventManager.h"

namespace EventManagerLib
{
    bool EventManager::RegisterEventDispatcher(EventDispatcher* dispatcher, std::shared_ptr<IEvent> eventKey)
    {
        auto it = std::find(m_eventDispatchers.begin(), m_eventDispatchers.end(), dispatcher);
        if (it != m_eventDispatchers.end())
            return false;
        m_dispatchIndices[eventKey->GetHashKey()] = m_eventDispatchers.size();
        m_eventDispatchers.push_back(dispatcher);
        return true;
    }

    bool EventManager::RegisterListener(IEventListener* callbackFunc, std::shared_ptr<IEvent> eventKey)
    {
        return m_eventDispatchers[m_dispatchIndices[eventKey->GetHashKey()]]->RegisterListener(callbackFunc);
    }

    void EventManager::RecordEvent(std::shared_ptr<IEvent> pEvent)
    {
        m_eventDispatchers[m_dispatchIndices[pEvent->GetHashKey()]]->RecordEvent(std::move(pEvent));
    }

    void EventManager::DispatchEvents()
    {
        for (EventDispatcher* dispatcher : m_eventDispatchers)
            dispatcher->DispatchEvents();
    }
}