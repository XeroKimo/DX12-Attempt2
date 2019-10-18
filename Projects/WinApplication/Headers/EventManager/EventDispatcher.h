#pragma once
#include "BaseEventDispatcher.h"
#include <vector>
#include <memory>

namespace WinApplication
{
    template<class Event, class Listener, class = std::enable_if_t<std::is_base_of_v<IEvent, Event>>, class = std::enable_if_t<std::is_base_of_v<IEventListener, Listener>>>
    class EventDispatcher : public EventDispatcherBase
    {
    private:
        std::vector<std::unique_ptr<Event>> m_eventsToDispatch;
        std::vector<Listener*> m_eventListeners;
    public:

        //// Inherited via EventDispatcherBase
        void DispatchEvents() override final
        {
            for (const std::unique_ptr<Event>& pEvent : m_eventsToDispatch)
            {
                for (Listener* listener : m_eventListeners)
                {
                    listener->OnEvent(pEvent.get());
                }
            }
            m_eventsToDispatch.clear();
        }

        void RecordEvent(std::unique_ptr<Event> pEvent)
        {
            m_eventsToDispatch.push_back(std::move(pEvent));
        }

        void RecordEvent(std::unique_ptr<IEvent> pEvent) override final
        {
#if _DEBUG
            std::unique_ptr<Event> cast(dynamic_cast<Event*>(pEvent.release()));

            if (!cast)
                return;
#else
            std::unique_ptr<Event> cast(static_cast<Event*>(pEvent.release()));
            //if (pEvent->GetHashKey() != typeid(Event))
            //    return;
#endif
            
            m_eventsToDispatch.push_back(std::move(cast));
        }

        bool RegisterListener(Listener* listener)
        {
            auto it = std::find(m_eventListeners.begin(), m_eventListeners.end(), listener);
            if (it != m_eventListeners.end())
                return false;

            m_eventListeners.push_back(listener);
            return true;
        }

        bool RegisterListener(IEventListener* listener) override final
        {
#if _DEBUG
            if (!dynamic_cast<Listener*>(listener))
                return false;
#endif
            auto it = std::find(m_eventListeners.begin(), m_eventListeners.end(), static_cast<Listener*>(listener));
            if (it != m_eventListeners.end())
                return false;

            m_eventListeners.push_back(static_cast<Listener*>(listener));
            return true;
        }
    };
}