#pragma once
#include "WinAppHelpers.h"
#include "../Events/Event.h"
#include <Windows.h>
#include <unordered_map>

namespace WinApplication
{
    using namespace Delegates;
    class WNDProcPassthrough
    {
        class WndProcEvent : public Event<void(WPARAM, LPARAM)> {};
    public:
        std::unordered_map<unsigned int, WndProcEvent> m_mappedEvents;
    public:
        LRESULT Invoke(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
        {
            if (m_mappedEvents.find(message) == m_mappedEvents.end())
                return DefWindowProc(hwnd, message, wParam, lParam);
            m_mappedEvents[message](wParam, lParam);
            return 0;
        }
    };

    class Window
    {
    public:
        Window();
        ~Window();
        bool Initialize(WNDCLASS winClass, CreateWindowHelper windowOptions);
        bool Initialize(WNDCLASS winClass, CreateWindowHelperEX windowOptions);

        bool Initialize(WNDCLASSEX winClass, CreateWindowHelper windowOptions);
        bool Initialize(WNDCLASSEX winClass, CreateWindowHelperEX windowOptions);

        inline LONG_PTR SetUserData(LONG_PTR ptr) { return SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, ptr); }
        LONG_PTR SetWindowLPTR(int nIndex, LONG_PTR ptr);

        static bool PeekMsg(MSG& msg, HWND hwnd = nullptr, UINT filterMin = 0, UINT filterMax = 0, UINT removeMsg = PM_REMOVE);
        static void ReadMsg(MSG& msg);

        void Quit();

        bool IsRunning() { return m_running; }
        HWND GetWindowHandle() { return m_windowHandle; }

        int GetWindowWidth();
        int GetWindowHeight();
        void GetWindowSize(int& outWidth, int& outHeight);
    private:
        HWND InitWindow(CreateWindowHelper windowOptions);
        HWND InitWindow(CreateWindowHelperEX windowOptions);

        //WNDPROC definiton
        //LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:
        WNDProcPassthrough passThrough;
    private:
        HWND m_windowHandle = nullptr;
        HINSTANCE m_hInstance = nullptr;
        bool m_running = false;
#ifdef _MBCS
        TDSTR m_className = "";
#elif _UNICODE
        TDSTR m_className = L"";
#endif // _MBCS
    };
}