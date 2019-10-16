#include "Windows/Application.h"

namespace WinApplication
{
    void Application::Run(IApp* app, Window* window)
    {
        m_mainWindow = window;
        if (m_mainWindow)
        {
            m_isRunning = m_mainWindow->GetWindowHandle();
        }
        if (m_isRunning)
            app->Initialize();

        MSG msg;
        while (m_isRunning)
        {
            if (m_mainWindow->PeekMsg(msg))
            {
                if (msg.message == WM_QUIT)
                    m_isRunning = false;
                m_mainWindow->ReadMsg(msg);
            }
            else
            {
                m_clock.Tick();
                eventManager->DispatchEvents();
                app->Update(m_clock.GetDeltaTime<float>());
                app->Draw();
            }
        }
    }
}