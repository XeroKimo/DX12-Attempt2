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


    class EventDispatcher
    {
    private:
        std::vector<std::unique_ptr<IEvent>> m_eventsToDispatch;
        std::vector<IEventListener*> m_eventListeners;
    public:
        EventDispatcher();

        void DispatchEvents();
        void RecordEvent(std::unique_ptr<IEvent> pEvent);
        bool RegisterListener(IEventListener* listener);

    };
}