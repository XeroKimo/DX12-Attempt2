#pragma once
#include "WinAppHelpers.h"
#include <Windows.h>

class WinApp
{
public:
    WinApp();
	~WinApp();
	bool Initialize(WNDCLASS winClass, WinAppHelpers::CreateWindowHelper windowOptions);
	bool Initialize(WNDCLASS winClass, WinAppHelpers::CreateWindowHelperEX windowOptions);

	bool Initialize(WNDCLASSEX winClass, WinAppHelpers::CreateWindowHelper windowOptions);
	bool Initialize(WNDCLASSEX winClass, WinAppHelpers::CreateWindowHelperEX windowOptions);

    inline LONG_PTR SetUserData(LONG_PTR ptr) { return SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, ptr); }
    LONG_PTR SetWindowLPTR(int nIndex, LONG_PTR ptr);

	static bool PeekMsg(MSG& msg, HWND hwnd = nullptr, UINT filterMin = 0, UINT filterMax = 0, UINT removeMsg = PM_REMOVE);
	static void ReadMsg(MSG& msg);

	void Quit();

	bool IsRunning() { return m_running; }
	HWND GetHandle() { return m_windowHandle; }

	int GetWindowWidth();
	int GetWindowHeight();
	void GetWindowSize(int& outWidth, int& outHeight);
private:
	HWND InitWindow(WinAppHelpers::CreateWindowHelper windowOptions);
	HWND InitWindow(WinAppHelpers::CreateWindowHelperEX windowOptions);

	//WNDPROC definiton
	//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_windowHandle;
    HINSTANCE m_hInstance;
	bool m_running;
    WinAppHelpers::TDSTR m_className;
};