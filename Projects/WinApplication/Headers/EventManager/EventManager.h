#pragma once
#include "EventDispatcher.h"
#include <unordered_map>
#include <typeindex>

namespace WinApplication
{

    class EventManager
    {
    private:
        std::vector<std::unique_ptr<EventDispatcherBase>> m_eventDispatchers;
        std::unordered_map<std::type_index, unsigned int> m_registeredEventTypes;
        std::unordered_map<std::type_index, unsigned int> m_registeredListenerTypes;

    public:
        void DispatchEvents();

        template<class BaseEvent, class Listener, class = std::enable_if_t<std::is_base_of_v<IEvent, BaseEvent>>, class = std::enable_if_t<std::is_base_of_v<IEventListener, Listener>>>
        bool RegisterEventDispatcher()
        {
            auto it = m_registeredEventTypes.find(typeid(BaseEvent*));
            if (it != m_registeredEventTypes.end())
                return false;

            it = m_registeredListenerTypes.find(typeid(Listener*));
            if (it != m_registeredListenerTypes.end())
                return false;

            m_registeredEventTypes[typeid(BaseEvent*)] = static_cast<unsigned int>(m_eventDispatchers.size());
            m_registeredListenerTypes[typeid(Listener*)] = static_cast<unsigned int>(m_eventDispatchers.size());
            m_eventDispatchers.push_back(std::make_unique<EventDispatcher<BaseEvent, Listener>>());
            return true;
        }

        template<class Listener, class = std::enable_if_t<std::is_base_of_v<IEventListener, Listener>>>
        bool RegisterListener(Listener* listener)
        {
            auto it = m_registeredListenerTypes.find(typeid(Listener*));
            if (it == m_registeredListenerTypes.end())
                return false;

            return m_eventDispatchers[m_registeredListenerTypes[typeid(Listener*)]]->RegisterListener(listener);
        }

        template<class BaseEvent, class = std::enable_if_t<std::is_base_of_v<IEvent, BaseEvent>>>
        void RecordEvent(std::unique_ptr<BaseEvent> pEvent)
        {
            auto it = m_registeredEventTypes.find(typeid(BaseEvent*));
            if (it == m_registeredEventTypes.end())
                return;

            m_eventDispatchers[m_registeredEventTypes[typeid(BaseEvent*)]]->RecordEvent(std::move(pEvent));
        }
    };
}