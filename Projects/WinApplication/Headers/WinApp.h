#pragma once
#include <Windows.h>

class WinApp
{
public:
    WinApp();
	~WinApp();
	bool Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height);

	bool PeekMsg(MSG& msg);
	void ReadMessage(MSG& msg);
	void Quit();
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool IsRunning() { return m_running; }

	HWND GetHandle() { return m_windowHandle; }
	
	int GetWindowWidth() { return m_windowWidth; }
	int GetWindowHeight() { return m_windowHeight; }
private:
	bool InitWindow(HINSTANCE hInstance, unsigned int width, unsigned int height);


private:
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;

	HWND m_windowHandle;
    HINSTANCE m_hInstance;
	bool m_running;
};