#pragma once
#include <typeindex>
#include <memory>

namespace WinApplication
{
    __interface IEvent
    {
        //When implemented, make it final so inherited classes can't override it
        std::type_index GetBaseTypeIndex();
    };



    template<class Event, class = std::enable_if_t<std::is_base_of_v<IEvent, Event>>>
    __interface IEventListener
    {
        void OnEvent(Event* pEvent);
    };



    class EventDispatcherBase
    {
    public:
        virtual void DispatchEvents() = 0;
    };
}