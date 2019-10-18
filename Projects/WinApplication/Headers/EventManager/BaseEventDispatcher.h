#pragma once
#include <typeindex>
#include <memory>

namespace WinApplication
{
    __interface IEvent
    {
        std::type_index GetTypeIndex();
    };

    __interface IEventListener
    {
        void OnEvent(IEvent* pEvent);
    };

    __interface EventDispatcherBase
    {
        void DispatchEvents();
        void RecordEvent(std::unique_ptr<IEvent> pEvent);
        bool RegisterListener(IEventListener* listener);
    };
}