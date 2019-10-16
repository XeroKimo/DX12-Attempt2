#pragma once
#include <string>

namespace EventManagerLib
{
    __interface IEvent
    {
        const std::string GetHashKey();
    };
}