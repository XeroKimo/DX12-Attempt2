#pragma once
#include <Windows.h>

namespace WinAppHelpers
{
#ifdef _MBCS
	using TDSTR = LPCSTR;
#elif _UNICODE
	using TDSTR = LPCWSTR;
#endif // _MBCS


	struct CreateWindowHelper
	{
		TDSTR    lpClassName;
		TDSTR    lpWindowName;
		DWORD     dwStyle;
		int       X;
		int       Y;
		int       nWidth;
		int       nHeight;
		HWND      hWndParent;
		HMENU     hMenu;
		HINSTANCE hInstance;
		LPVOID    lpParam;

        inline void ConvertToClientSize()
        {
            RECT rect = {};
            rect.bottom = nHeight;
            rect.right = nWidth;
            AdjustWindowRect(&rect, dwStyle, false);

            nHeight = rect.bottom - rect.top;
            nWidth = rect.right - rect.left;

            X += rect.left;
            Y += rect.top;
        }

        inline void CenterWindow()
        {
            CenterWindow(nWidth, nHeight, X, Y);
        }

        static CreateWindowHelper Standard(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
		{
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

			CreateWindowHelper helper =
			{
				lpClassName,                     // Window class
				lpClassName,    // Window text
                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,           // Window style
				x, y,	// Position
                windowWidth, windowHeight,					// Size
				NULL,							// Parent window    
				NULL,							// Menu
				hInstance,					// Instance handle
			};

			return helper;
		}

        static CreateWindowHelper Borderless(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
        {
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

            CreateWindowHelper helper =
            {
                lpClassName,                     // Window class
                lpClassName,    // Window text
                WS_POPUP,            // Window style
                x, y,	// Position
                windowWidth, windowHeight,					// Size
                NULL,							// Parent window    
                NULL,							// Menu
                hInstance,					// Instance handle
            };

            return helper;
        }

        static void CenterWindow(int windowWidth, int windowHeight, int& outX, int& outY)
        {
            HWND window = GetDesktopWindow();
            RECT windowRect;
            GetWindowRect(window, &windowRect);

            outX = windowRect.right / 2 - windowWidth / 2;
            outY = windowRect.bottom / 2 - windowHeight / 2;
        }
	};

	struct CreateWindowHelperEX
	{
		DWORD     dwExStyle;
		TDSTR    lpClassName;
		TDSTR    lpWindowName;
		DWORD     dwStyle;
		int       X;
		int       Y;
		int       nWidth;
		int       nHeight;
		HWND      hWndParent;
		HMENU     hMenu;
		HINSTANCE hInstance;
		LPVOID    lpParam;

        inline void ConvertToClientSize()
        {
            RECT rect = {};
            rect.bottom = nHeight;
            rect.right = nWidth;
            AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle);

            nHeight = rect.bottom - rect.top;
            nWidth = rect.right - rect.left;

            X += rect.left;
            Y += rect.top;
        }

        inline void CenterWindow()
        {
            CenterWindow(nWidth, nHeight, X, Y);
        }

		static CreateWindowHelperEX Standard(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
		{
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

			CreateWindowHelperEX helper = 
			{
				NULL,                           // Optional window styles.
				lpClassName,                     // Window class
				lpClassName,    // Window text
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,            // Window style
				x, y,	// Position
				windowWidth, windowHeight,					// Size
				NULL,							// Parent window    
				NULL,							// Menu
				hInstance,					// Instance handle
				NULL							// Additional application data
			};

			return helper;
		}

        static CreateWindowHelperEX Borderless(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
        {
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

            CreateWindowHelperEX helper =
            {
                NULL,                           // Optional window styles.
                lpClassName,                     // Window class
                lpClassName,    // Window text
                WS_POPUP,            // Window style
                x,y,	// Position
                windowWidth, windowHeight,					// Size
                NULL,							// Parent window    
                NULL,							// Menu
                hInstance,					// Instance handle
                NULL							// Additional application data
            };

            return helper;
        }

        static void CenterWindow(int windowWidth, int windowHeight, int& outX, int& outY)
        {
            HWND window = GetDesktopWindow();
            RECT windowRect;
            GetWindowRect(window, &windowRect);

            outX = windowRect.right / 2 - windowWidth / 2;
            outY = windowRect.bottom / 2 - windowHeight / 2;
        }
	};

	inline WNDCLASS WndClassStandard(HINSTANCE hInstance, WNDPROC lpfnWndProc, TDSTR lpszClassName, HICON hIcon = LoadIcon(NULL, IDI_APPLICATION), HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW))
	{
		WNDCLASS wc;

		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = lpfnWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = lpszClassName;

		return wc;
	}

	inline WNDCLASSEX WndClassEXStandard(HINSTANCE hInstance, WNDPROC lpfnWndProc, TDSTR lpszClassName, HICON hIcon = LoadIcon(NULL, IDI_APPLICATION), HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW))
	{
		WNDCLASSEX wc;

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = lpfnWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = lpszClassName;
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		wc.hIconSm = NULL;

		return wc;
	}
}