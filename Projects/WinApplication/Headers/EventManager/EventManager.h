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
        std::unordered_map<std::type_index, int> m_registeredEventTypes;
        std::unordered_map<std::type_index, int> m_registeredListenerTypes;

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

            m_registeredEventTypes[typeid(BaseEvent*)] = m_eventDispatchers.size();
            m_registeredListenerTypes[typeid(Listener*)] = m_eventDispatchers.size();
            m_eventDispatchers.push_back(std::make_unique<EventDispatcher<BaseEvent, Listener>>());
            return true;
        }

        template<class Listener, class = std::enable_if_t<std::is_base_of_v<IEventListener, Listener>>>
        bool RegisterListener(IEventListener* listener)
        {
#if _DEBUG
            Listener* cast = dynamic_cast<Listener*>(listener);
            if (!cast)
                return false;
#elif
            Listener* cast = static_cast<Listener*>(listener);
#endif
            return m_eventDispatchers[m_registeredListenerTypes[typeid(cast)]]->RegisterListener(listener);
        }

        template<class BaseEvent, class = std::enable_if_t<std::is_base_of_v<IEvent, BaseEvent>>>
        void RecordEvent(std::unique_ptr<IEvent> pEvent)
        {
#if _DEBUG
            BaseEvent* cast = dynamic_cast<BaseEvent*>(pEvent.get());
            if (!cast)
                return;
#elif
            BaseEvent* cast = static_cast<BaseEvent*>(pEvent.get());
#endif
            m_eventDispatchers[m_registeredEventTypes[typeid(cast)]]->RecordEvent(std::move(pEvent));
        }
    };
}