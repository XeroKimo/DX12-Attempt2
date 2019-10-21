#pragma once
#include "BaseEventDispatcher.h"
#include <vector>
#include <memory>

namespace WinApplication
{
    template<class Event, class = std::enable_if_t<std::is_base_of_v<IEvent, Event>>>
    class EventDispatcher : public EventDispatcherBase
    {
    private:
        std::vector<std::unique_ptr<Event>> m_eventsToDispatch;
        std::vector<IEventListener<Event>*> m_eventListeners;
    public:

        void DispatchEvents() override final
        {
            for (const std::unique_ptr<Event>& pEvent : m_eventsToDispatch)
            {
                for (IEventListener<Event>* listener : m_eventListeners)
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

        bool RegisterListener(IEventListener<Event>* listener)
        {
            auto it = std::find(m_eventListeners.begin(), m_eventListeners.end(), listener);
            if (it != m_eventListeners.end())
                return false;

            m_eventListeners.push_back(listener);
            return true;
        }

        void ImmediateDispatcher(Event pEvent)
        {
            for (IEventListener<Event>* listener : m_eventListeners)
            {
                listener->OnEvent(&pEvent);
            }
        }
    };
}