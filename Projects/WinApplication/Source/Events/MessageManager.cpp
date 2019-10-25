#include "Events/MessageManager.h"

namespace WinApplication
{
    void MessageManager::DispatchEvents()
    {
        for (std::unique_ptr<MessageDispatcherBase>& dispatcher : m_eventDispatchers)
            dispatcher->DispatchEvents();
    }
}