#pragma once
#include <unordered_map>
#include <typeindex>
#include "EventDispatcher.h"

namespace WinApplication
{
    class EventManager
    {
    private:
        std::vector<std::unique_ptr<EventDispatcherBase>> m_eventDispatchers;
        std::unordered_map<std::type_index, int> m_registeredEventTypes;
        std::unordered_map<std::type_index, int> m_registeredListenerTypes;

    public:
        void RecordEvent(std::unique_ptr<IEvent> pEvent);
        void DispatchEvents();

        template<class Event, class Listener, class = std::enable_if_t<std::is_base_of_v<IEvent, Event>>, class = std::enable_if_t<std::is_base_of_v<IEventListener, Listener>>>
        bool RegisterEventDispatcher()
        {
            auto it = m_registeredEventTypes.find(typeid(Event));
            if (it != m_registeredEventTypes.end())
                return false;

            it = m_registeredListenerTypes.find(typeid(Listener));
            if (it != m_registeredListenerTypes.end())
                return false;

            m_registeredEventTypes[typeid(Event)] = m_eventDispatchers.size();
            m_registeredListenerTypes[typeid(Listener*)] = m_eventDispatchers.size();
            m_eventDispatchers.push_back(std::make_unique<EventDispatcher<Event, Listener>>());
            return true;
        }

        template<class Listener, class = std::enable_if_t<std::is_base_of_v<IEventListener, Listener>>>
        bool RegisterListener(IEventListener* listener)
        {
            Listener* cast = dynamic_cast<Listener*>(listener);
            if (!cast)
                return false;
            return m_eventDispatchers[m_registeredListenerTypes[typeid(cast)]]->RegisterListener(listener);
        }
    };
}