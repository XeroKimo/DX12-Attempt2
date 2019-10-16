#include "EventManager/EventDispatcher.h"
#include <assert.h>


namespace WinApplication
{
    EventDispatcher::EventDispatcher()
    {

    }

    void EventDispatcher::DispatchEvents()
    {
        for (const std::shared_ptr<IEvent>& pEvent : m_eventsToDispatch)
        {
            for (IEventListener* listener : m_eventListeners)
            {
                listener->OnEvent(pEvent.get());
            }
        }
        m_eventsToDispatch.clear();
    }

    void EventDispatcher::RecordEvent(std::shared_ptr<IEvent> pEvent)
    {
#if _DEBUG
        if (m_eventKey != pEvent->GetHashKey())
        {
            assert(false);
            return;
        }
#endif

        m_eventsToDispatch.push_back(std::move(pEvent));
    }

    bool EventDispatcher::RegisterListener(IEventListener* callbackFunc)
    {
        auto it = std::find(m_eventListeners.begin(), m_eventListeners.end(), callbackFunc);
        if (it != m_eventListeners.end())
            return false;

        m_eventListeners.push_back(callbackFunc);
        return true;
    }
}