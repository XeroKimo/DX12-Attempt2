#pragma once
#include "IEvent.h"
#include <vector>
#include <memory>

namespace WinApplication
{
    __interface IEventListener
    {
        void OnEvent(IEvent* pEvent);
    };

    __interface EventDispatcherBase
    {
        void DispatchEvents();
        void RecordEvent(std::unique_ptr<IEvent> pEvent);
        bool RegisterListener(IEventListener* listener);
    };

    template<class Event, class Listener, class = std::enable_if_t<std::is_base_of_v<IEvent, Event>>, class = std::enable_if_t<std::is_base_of_v<IEventListener, Listener>>>
    class EventDispatcher : public EventDispatcherBase
    {
    private:
        std::vector<std::unique_ptr<IEvent>> m_eventsToDispatch;
        std::vector<IEventListener*> m_eventListeners;
    public:

        //// Inherited via EventDispatcherBase
        void DispatchEvents() override final
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

        void RecordEvent(std::unique_ptr<IEvent> pEvent) override final
        {
#if _DEBUG
            if (pEvent->GetHashKey() != typeid(Event))
                return;
#endif
            m_eventsToDispatch.push_back(std::move(pEvent));
        }

        bool RegisterListener(IEventListener* listener) override final
        {
#if _DEBUG
            if (!dynamic_cast<Listener*>(listener))
                return false;
#endif
            auto it = std::find(m_eventListeners.begin(), m_eventListeners.end(), listener);
            if (it != m_eventListeners.end())
                return false;

            m_eventListeners.push_back(listener);
            return true;
        }
    };
}