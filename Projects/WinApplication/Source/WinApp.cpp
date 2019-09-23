#include <WinApp.h>

WinApp::WinApp() :
	m_hInstance(nullptr),
	m_windowHandle(nullptr),
	m_windowHeight(0),
	m_windowWidth(0),
	m_running(false)
{
}

WinApp::~WinApp()
{
	DestroyWindow(m_windowHandle);
	UnregisterClass(L"Sample Window Class", m_hInstance);
}

bool WinApp::Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height)
{
	m_windowWidth = width;
	m_windowHeight = height;
	return m_running = InitWindow(hInstance, width, height); 
}

bool WinApp::InitWindow(HINSTANCE hInstance, unsigned int width, unsigned  int height)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"DX12 Practice";
    WNDCLASS wc = {};

	//m_hInstance = GetModuleHandle(nullptr);
	m_hInstance = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
    wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
		return false;
	}
    // Create the window.

    m_windowHandle = CreateWindowEx(
        NULL,                           // Optional window styles.
        CLASS_NAME,                     // Window class
		CLASS_NAME,    // Window text
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,	// Size
		width, height,					// Position
        NULL,							// Parent window    
        NULL,							// Menu
        m_hInstance,					// Instance handle
        NULL							// Additional application data
    );

    if (!m_windowHandle)
    {
		MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
        return false;
    }

	SetWindowLongPtr(m_windowHandle, GWL_USERDATA, (LONG_PTR)this);

    ShowWindow(m_windowHandle, SW_SHOW);
	SetForegroundWindow(m_windowHandle);
	SetFocus(m_windowHandle);
    return true;
}

void WinApp::ReadMessage(MSG& msg)
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

void WinApp::Quit()
{
	m_running = false;
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp* pWinApp = (WinApp*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	// Get a pointer to the framework object associated with this window.
    switch (uMsg)
    {
	case WM_NCCREATE:
	{
		// Set the user data for this hWnd to the Framework* we passed in, used on first line of this method above.
		//CREATESTRUCT* pcs = (CREATESTRUCT*)(lParam);
		//WinApp* pFramework = (WinApp*)(pcs->lpCreateParams);
		//SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(pFramework));
		return 1;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(0, L"Are you sure you want to exit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
				DestroyWindow(hwnd);
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}