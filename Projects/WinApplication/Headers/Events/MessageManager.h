#pragma once
#include "MessageDispatcher.h"
#include <unordered_map>
#include <typeindex>

namespace WinApplication
{

    class MessageManager
    {
    private:
        std::vector<std::unique_ptr<MessageDispatcherBase>> m_eventDispatchers;
        std::unordered_map<std::type_index, unsigned int> m_registeredEventTypes;

    public:
        void DispatchEvents();

        template<class Message, class = std::enable_if_t<std::is_base_of_v<IMessage, Message>>>
        bool RegisterEventDispatcher()
        {
            auto it = m_registeredEventTypes.find(typeid(Message*));
            if (it != m_registeredEventTypes.end())
                return false;

            m_registeredEventTypes[typeid(Message*)] = static_cast<unsigned int>(m_eventDispatchers.size());
            m_eventDispatchers.push_back(std::make_unique<MessageDispatcher<Message>>());
            return true;
        }

        template<class Message, class = std::enable_if_t<std::is_base_of_v<IMessage, Message>>>
        void RegisterListener(Delegate<void(Message*)> delegate)
        {
            auto it = m_registeredEventTypes.find(typeid(Message*));
            if (it == m_registeredEventTypes.end())
                return;

            static_cast<MessageDispatcher<Message, void>*>(m_eventDispatchers[m_registeredEventTypes[typeid(Message*)]].get())->RegisterListener(delegate);
        }

        template<class Message, class = std::enable_if_t<std::is_base_of_v<IMessage, Message>>>
        void RecordEvent(std::unique_ptr<Message> pEvent)
        {
            auto it = m_registeredEventTypes.find(typeid(Message*));
            if (it == m_registeredEventTypes.end())
                return;

            static_cast<MessageDispatcher<Message, void>*>(m_eventDispatchers[m_registeredEventTypes[typeid(Message*)]].get())->RecordMessage(std::move(pEvent));
        }
    };
}