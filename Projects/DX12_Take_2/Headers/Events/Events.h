#pragma once
#include <Windows.h>
#include "EventManager/BaseEventDispatcher.h"

class EventWindows final : public WinApplication::IEvent
{
private:
    MSG m_msg;
    LPARAM m_lparam;
    WPARAM m_wparam;
public:
    EventWindows(MSG msg, LPARAM lparam, WPARAM wparam)
    {
        m_msg = msg;
        m_lparam = lparam;
        m_wparam = wparam;
    }

    const MSG& GetMsg() { return m_msg; }
    const LPARAM& GetLPARAM() { return m_lparam; }
    const WPARAM& GetWPARAM() { return m_wparam; }
};