#pragma once
#include "BaseMessageDispatcher.h"
#include "Event.h"
#include <vector>
#include <memory>

namespace WinApplication
{
    using namespace Delegates;
    template<class Message, class = std::enable_if_t<std::is_base_of_v<IMessage, Message>>>
    class MessageDispatcher : public MessageDispatcherBase
    {
    private:
        std::vector<std::unique_ptr<Message>> m_messagesToDispatch;
        Event<void(Message*)> m_messageListeners;
    public:

        void DispatchEvents() override final
        {
            for (const std::unique_ptr<Message>& pEvent : m_messagesToDispatch)
            {
                m_messageListeners(pEvent.get());
            }
            m_messagesToDispatch.clear();
        }

        void RecordMessage(std::unique_ptr<Message> pEvent)
        {
            m_messagesToDispatch.push_back(std::move(pEvent));
        }

        template<class T, Delegates::Internal::MethodPtr<T, void, Message*> func>
        void RegisterListener(T* object)
        {
            m_messageListeners.Bind<T, func>(object);
            //m_messageListeners += delegate;
        }
    };
}