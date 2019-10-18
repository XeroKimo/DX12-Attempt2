#include "EventManager/EventDispatcher.h"
#include <assert.h>


namespace WinApplication
{
    EventDispatcher::EventDispatcher()
    {

    }

    void EventDispatcher::DispatchEvents()
    {
        for (const std::unique_ptr<IEvent>& pEvent : m_eventsToDispatch)
        {
            for (IEventListener* listener : m_eventListeners)
            {
                listener->OnEvent(pEvent.get());
            }
        }
        m_eventsToDispatch.clear();
    }

    void EventDispatcher::RecordEvent(std::unique_ptr<IEvent> pEvent)
    {
        m_eventsToDispatch.push_back(std::move(pEvent));
    }

    bool EventDispatcher::RegisterListener(IEventListener* listener)
    {
        auto it = std::find(m_eventListeners.begin(), m_eventListeners.end(), listener);
        if (it != m_eventListeners.end())
            return false;

        m_eventListeners.push_back(listener);
        return true;
    }
}