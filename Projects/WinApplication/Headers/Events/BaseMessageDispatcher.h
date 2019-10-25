#pragma once
#include <typeindex>
#include <memory>

namespace WinApplication
{
    __interface IMessage
    {
        //When implemented, make it final so inherited classes can't override it
        std::type_index GetBaseTypeIndex();
    };

    class MessageDispatcherBase
    {
    public:
        virtual void DispatchEvents() = 0;
    };
}