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


    class EventDispatcher : public IEventListener
    {
    protected:
    private:
        std::vector<std::shared_ptr<IEvent>> m_eventsToDispatch;
        std::vector<IEventListener*> m_eventListeners;
        std::string m_eventKey;
    public:
        EventDispatcher();

        void DispatchEvents();
        void RecordEvent(std::shared_ptr<IEvent> pEvent);
        bool RegisterListener(IEventListener* callbackFunc);
        virtual void OnEvent(IEvent* pEvent) = 0;
        void EventTest(IEvent* pEvent) {}

        void RegisterEventKey(std::string eventKey) { m_eventKey = eventKey; }
        const std::string GetEventHashKey() { return m_eventKey; }
    };
}