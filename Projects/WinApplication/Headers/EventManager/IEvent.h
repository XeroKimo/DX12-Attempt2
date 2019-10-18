#pragma once
#include <string>
#include <typeindex>

namespace WinApplication
{
    __interface IEvent
    {
        const std::type_index GetHashKey();
    };
}