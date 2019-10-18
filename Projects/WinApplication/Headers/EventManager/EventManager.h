#pragma once
#include <unordered_map>
#include <typeindex>
#include "EventDispatcher.h"

namespace WinApplication
{
    class EventManager
    {
    private:
        std::vector<EventDispatcher*> m_eventDispatchers;
        std::unordered_map<std::type_index, int> m_dispatchIndices;
    public:
        bool RegisterEventDispatcher(EventDispatcher* dispatcher, std::unique_ptr<IEvent> eventKey);
        bool RegisterListener(IEventListener* callbackFunc, std::shared_ptr<IEvent> eventKey);
        void RecordEvent(std::unique_ptr<IEvent> pEvent);
        void DispatchEvents();

    private:
        bool IsKeyRegistered(IEvent* pEvent);
    };
}