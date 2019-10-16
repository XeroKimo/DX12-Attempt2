#pragma once
#include <unordered_map>
#include "EventDispatcher.h"

namespace WinApplication
{
    class EventManager
    {
    private:
        std::vector<EventDispatcher*> m_eventDispatchers;
        std::unordered_map<std::string, int> m_dispatchIndices;
    public:
        bool RegisterEventDispatcher(EventDispatcher* dispatcher, std::shared_ptr<IEvent> eventKey);
        bool RegisterListener(IEventListener* callbackFunc, std::shared_ptr<IEvent> eventKey);
        void RecordEvent(std::shared_ptr<IEvent> pEvent);
        void DispatchEvents();

    };
}