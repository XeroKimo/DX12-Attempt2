#pragma once
#include <memory>
#include "PlatformClock.h"
#include "Window.h"
#include "Events/MessageManager.h"

namespace WinApplication
{
    __interface IApp
    {
        void Initialize();
        void Update(float deltaTime);
        void Draw();
    };

    class Application
    {
    public:
        std::shared_ptr<MessageManager> eventManager;
    private:
        PlatformClock m_clock = {};
        bool m_isRunning = false;
        Window* m_mainWindow = nullptr;
    public:
        void Run(IApp* app, Window* window);
    };
}