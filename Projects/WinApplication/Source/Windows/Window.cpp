#include "Windows/Window.h"

namespace WinApplication
{
    Window::Window()
    {
    }

    Window::~Window()
    {
        DestroyWindow(m_windowHandle);
        UnregisterClass(m_className, m_hInstance);
    }

    bool Window::Initialize(WNDCLASS winClass, CreateWindowHelper windowOptions)
    {
        if (!RegisterClass(&winClass))
        {
            MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
            return false;
        }

        m_windowHandle = InitWindow(windowOptions);
        if (!m_windowHandle)
        {
            MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
            return false;
        }

        SetUserData(reinterpret_cast<LONG_PTR>(&passThrough));
        ShowWindow(m_windowHandle, SW_SHOW);
        SetForegroundWindow(m_windowHandle);
        SetFocus(m_windowHandle);

        m_running = true;
        return true;
    }

    bool Window::Initialize(WNDCLASS winClass, CreateWindowHelperEX windowOptions)
    {
        if (!RegisterClass(&winClass))
        {
            MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
            return false;
        }

        m_windowHandle = InitWindow(windowOptions);
        if (!m_windowHandle)
        {
            MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
            return false;
        }

        ShowWindow(m_windowHandle, SW_SHOW);
        SetForegroundWindow(m_windowHandle);
        SetFocus(m_windowHandle);

        m_running = true;
        return true;
    }

    bool Window::Initialize(WNDCLASSEX winClass, CreateWindowHelper windowOptions)
    {
        if (!RegisterClassEx(&winClass))
        {
            MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
            return false;
        }

        m_windowHandle = InitWindow(windowOptions);
        if (!m_windowHandle)
        {
            MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
            return false;
        }

        ShowWindow(m_windowHandle, SW_SHOW);
        SetForegroundWindow(m_windowHandle);
        SetFocus(m_windowHandle);

        m_running = true;
        return true;
    }

    bool Window::Initialize(WNDCLASSEX winClass, CreateWindowHelperEX windowOptions)
    {
        if (!RegisterClassEx(&winClass))
        {
            MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
            return false;
        }

        m_windowHandle = InitWindow(windowOptions);
        if (!m_windowHandle)
        {
            MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
            return false;
        }

        ShowWindow(m_windowHandle, SW_SHOW);
        SetForegroundWindow(m_windowHandle);
        SetFocus(m_windowHandle);

        m_running = true;
        return true;
    }

    LONG_PTR Window::SetWindowLPTR(int nIndex, LONG_PTR ptr)
    {
        return SetWindowLongPtr(m_windowHandle, nIndex, ptr);
    }

    int Window::GetWindowWidth()
    {
        int width, height;
        GetWindowSize(width, height);
        return width;
    }

    int Window::GetWindowHeight()
    {
        int width, height;
        GetWindowSize(width, height);
        return height;
    }

    void Window::GetWindowSize(int& outWidth, int& outHeight)
    {

        RECT rect;
        if (!GetClientRect(m_windowHandle, &rect))
            return;
        outWidth = rect.right;
        outHeight = rect.bottom;
    }

    HWND Window::InitWindow(CreateWindowHelper windowOptions)
    {
        m_className = windowOptions.lpClassName;
        m_hInstance = windowOptions.hInstance;
        return CreateWindow
        (
            windowOptions.lpClassName,
            windowOptions.lpWindowName,
            windowOptions.dwStyle,
            windowOptions.X,
            windowOptions.Y,
            windowOptions.nWidth,
            windowOptions.nHeight,
            windowOptions.hWndParent,
            windowOptions.hMenu,
            windowOptions.hInstance,
            windowOptions.lpParam
        );
    }

    HWND Window::InitWindow(CreateWindowHelperEX windowOptions)
    {
        m_className = windowOptions.lpClassName;
        m_hInstance = windowOptions.hInstance;
        return CreateWindowEx
        (
            windowOptions.dwExStyle,
            windowOptions.lpClassName,
            windowOptions.lpWindowName,
            windowOptions.dwStyle,
            windowOptions.X,
            windowOptions.Y,
            windowOptions.nWidth,
            windowOptions.nHeight,
            windowOptions.hWndParent,
            windowOptions.hMenu,
            windowOptions.hInstance,
            windowOptions.lpParam
        );
    }

    bool Window::PeekMsg(MSG& msg, HWND hwnd, UINT filterMin, UINT filterMax, UINT removeMsg)
    {
        return (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE));
    }

    void Window::ReadMsg(MSG& msg)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    void Window::Quit()
    {
        m_running = false;
    }
}