#include <WinApp.h>

WinApp::WinApp() :
	m_hInstance(nullptr),
	m_windowHandle(nullptr),
	m_running(false)
{
}

WinApp::~WinApp()
{
	DestroyWindow(m_windowHandle);
	UnregisterClass(L"Sample Window Class", m_hInstance);
}

bool WinApp::Initialize(WNDCLASS winClass, WinAppHelpers::CreateWindowHelper windowOptions)
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

bool WinApp::Initialize(WNDCLASS winClass, WinAppHelpers::CreateWindowHelperEX windowOptions)
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

bool WinApp::Initialize(WNDCLASSEX winClass, WinAppHelpers::CreateWindowHelper windowOptions)
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

bool WinApp::Initialize(WNDCLASSEX winClass, WinAppHelpers::CreateWindowHelperEX windowOptions)
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

LONG_PTR WinApp::SetWindowLPTR(int nIndex, LONG_PTR ptr)
{
    return SetWindowLongPtr(m_windowHandle, nIndex, ptr);
}

int WinApp::GetWindowWidth()
{
	int width, height;
	GetWindowSize(width, height);
	return width;
}

int WinApp::GetWindowHeight()
{
	int width, height;
	GetWindowSize(width, height);
	return height;
}

void WinApp::GetWindowSize(int& outWidth, int& outHeight)
{

	RECT rect;
	if (!GetClientRect(m_windowHandle, &rect))
		return;
	outWidth = rect.right;
	outHeight = rect.bottom;
}

HWND WinApp::InitWindow(WinAppHelpers::CreateWindowHelper windowOptions)
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

HWND WinApp::InitWindow(WinAppHelpers::CreateWindowHelperEX windowOptions)
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

bool WinApp::PeekMsg(MSG& msg, HWND hwnd, UINT filterMin, UINT filterMax, UINT removeMsg)
{
	return (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE));
}

void WinApp::ReadMsg(MSG& msg)
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

void WinApp::Quit()
{
	m_running = false;
}
