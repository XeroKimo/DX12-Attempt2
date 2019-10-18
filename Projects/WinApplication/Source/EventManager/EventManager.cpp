#include "EventManager/EventManager.h"

namespace WinApplication
{
    bool EventManager::RegisterEventDispatcher(EventDispatcher* dispatcher, std::unique_ptr<IEvent> eventKey)
    {
        auto it = std::find(m_eventDispatchers.begin(), m_eventDispatchers.end(), dispatcher);
        if (it != m_eventDispatchers.end())
            return false;

#if _DEBUG
        if (IsKeyRegistered(eventKey.get()))
            return false;
#endif

        m_dispatchIndices[eventKey->GetHashKey()] = m_eventDispatchers.size();
        m_eventDispatchers.push_back(dispatcher);
        return true;
    }

    bool EventManager::RegisterListener(IEventListener* callbackFunc, std::shared_ptr<IEvent> eventKey)
    {
        if (IsKeyRegistered(eventKey.get()))
            return m_eventDispatchers[m_dispatchIndices[eventKey->GetHashKey()]]->RegisterListener(callbackFunc);
        return false;
    }

    void EventManager::RecordEvent(std::unique_ptr<IEvent> pEvent)
    {
        if (IsKeyRegistered(pEvent.get()))
            m_eventDispatchers[m_dispatchIndices[pEvent->GetHashKey()]]->RecordEvent(std::move(pEvent));
    }

    void EventManager::DispatchEvents()
    {
        for (EventDispatcher* dispatcher : m_eventDispatchers)
            dispatcher->DispatchEvents();
    }

    bool EventManager::IsKeyRegistered(IEvent* pEvent)
    {
#if _DEBUG
        auto it = m_dispatchIndices.find(pEvent->GetHashKey());
        return it != m_dispatchIndices.end();
#endif
        return true;
    }
}