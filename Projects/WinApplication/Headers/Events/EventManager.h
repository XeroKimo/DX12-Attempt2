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

    public:
        void DispatchEvents();

        template<class BaseEvent, class = std::enable_if_t<std::is_base_of_v<IEvent, BaseEvent>>>
        bool RegisterEventDispatcher()
        {
            auto it = m_registeredEventTypes.find(typeid(BaseEvent*));
            if (it != m_registeredEventTypes.end())
                return false;

            m_registeredEventTypes[typeid(BaseEvent*)] = static_cast<unsigned int>(m_eventDispatchers.size());
            m_eventDispatchers.push_back(std::make_unique<EventDispatcher<BaseEvent>>());
            return true;
        }

        template<class BaseEvent, class = std::enable_if_t<std::is_base_of_v<IEvent, BaseEvent>>>
        bool RegisterListener(IEventListener<BaseEvent>* listener)
        {
            auto it = m_registeredEventTypes.find(typeid(BaseEvent*));
            if (it == m_registeredEventTypes.end())
                return false;

            return static_cast<EventDispatcher<BaseEvent, void>*>(m_eventDispatchers[m_registeredEventTypes[typeid(BaseEvent*)]].get())->RegisterListener(listener);
        }

        template<class BaseEvent, class = std::enable_if_t<std::is_base_of_v<IEvent, BaseEvent>>>
        void RecordEvent(std::unique_ptr<BaseEvent> pEvent)
        {
            auto it = m_registeredEventTypes.find(typeid(BaseEvent*));
            if (it == m_registeredEventTypes.end())
                return;

            static_cast<EventDispatcher<BaseEvent, void>*>(m_eventDispatchers[m_registeredEventTypes[typeid(BaseEvent*)]].get())->RecordEvent(std::move(pEvent));
        }
    };
}