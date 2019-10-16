#include "Application.h"

namespace WinApplication
{
    void Application::Run(IApp* app, Window* window)
    {
        m_mainWindow = window;
        if (m_mainWindow)
        {
            m_isRunning = m_mainWindow->GetWindowHandle();
        }

        MSG msg;
        while (m_isRunning)
        {
            if (m_mainWindow->PeekMsg(msg))
            {
                m_mainWindow->ReadMsg(msg);
            }
            else
            {
                m_clock.Tick();
                app->Update(m_clock.GetDeltaTime<float>());
                app->Draw();
            }
        }

    }
}