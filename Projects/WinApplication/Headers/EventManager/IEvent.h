#pragma once
#include <string>

namespace WinApplication
{
    __interface IEvent
    {
        const std::string GetHashKey();
    };
}