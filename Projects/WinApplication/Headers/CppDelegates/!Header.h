#pragma once

#include "Delegate.h"
#include "MultiCastDelegate.h"

namespace WinApplication
{
    template <class RET, class ...PARAMS>
    template <class FUNC = RET(PARAMS...)>
    using Event = SA::multicast_delegate<FUNC>;

    template <class RET, class ...PARAMS>
    template <class FUNC = RET(PARAMS...)>
    using Delegate = SA::delegate<FUNC>;
}