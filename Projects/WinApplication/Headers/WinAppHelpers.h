#pragma once
#include <Windows.h>

namespace WinApplication
{
#ifdef _MBCS
    using TDSTR = LPCSTR;
#elif _UNICODE
    using TDSTR = LPCWSTR;
#endif // _MBCS

    //https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
    enum class Window_Style : long long
    {
        Border = WS_BORDER,
        Caption = WS_CAPTION,
        Child = WS_CHILD,
        Child_Window = WS_CHILDWINDOW,
        Clip_Children = WS_CLIPCHILDREN,
        Clip_Siblings = WS_CLIPSIBLINGS,
        Disabled = WS_DISABLED,
        DLG_Frame = WS_DLGFRAME,
        Group = WS_GROUP,
        H_Scroll = WS_HSCROLL,
        Iconic = WS_ICONIC,
        Maximize = WS_MAXIMIZE,
        Maximize_Box = WS_MAXIMIZEBOX,
        Minimize = WS_MINIMIZE,
        Minimize_Box = WS_MINIMIZEBOX,
        Overlapped = WS_OVERLAPPED,
        Overlapped_Window = WS_OVERLAPPEDWINDOW,
        Pop_Up = WS_POPUP,
        Pop_Up_Window = WS_POPUPWINDOW,
        Size_Box = WS_SIZEBOX,
        Sys_Menu = WS_SYSMENU,
        Tab_Stop = WS_TABSTOP,
        Thick_Frame = WS_THICKFRAME,
        Tiled = WS_TILED,
        Tiled_Window = WS_TILEDWINDOW,
        Visible = WS_VISIBLE,
        V_Scroll = WS_VSCROLL,
    };
    DEFINE_ENUM_FLAG_OPERATORS(Window_Style);

    //https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
    enum class Window_Style_EX : long long
    {
        None = 0,
        Accept_Files = WS_EX_ACCEPTFILES,
        App_Window = WS_EX_APPWINDOW,
        Client_Edge = WS_EX_CLIENTEDGE,
        Composited = WS_EX_COMPOSITED,
        Context_Help = WS_EX_CONTEXTHELP,
        Control_Parent = WS_EX_CONTROLPARENT,
        DLG_Modal_Frame = WS_EX_DLGMODALFRAME,
        Layered = WS_EX_LAYERED,
        Layour_RTL = WS_EX_LAYOUTRTL,
        Left = WS_EX_LEFT,
        Left_Scroll_Bar = WS_EX_LEFTSCROLLBAR,
        LTR_Reading = WS_EX_LTRREADING,
        MDI_Child = WS_EX_MDICHILD,
        No_Activate = WS_EX_NOACTIVATE,
        No_Inherit_Layout = WS_EX_NOINHERITLAYOUT,
        No_Parent_Notify = WS_EX_NOPARENTNOTIFY,
        No_Redirection_Bitmap = WS_EX_NOREDIRECTIONBITMAP,
        Overlapped_Window = WS_EX_OVERLAPPEDWINDOW,
        Palette_Window = WS_EX_PALETTEWINDOW,
        Right = WS_EX_RIGHT,
        Right_Scroll_Bar = WS_EX_RIGHTSCROLLBAR,
        RTL_Reading = WS_EX_RTLREADING,
        Static_Edge = WS_EX_STATICEDGE,
        Tool_Window = WS_EX_TOOLWINDOW,
        Top_Most = WS_EX_TOPMOST,
        Transparent = WS_EX_TRANSPARENT,
        Window_Edge = WS_EX_WINDOWEDGE
    };
    DEFINE_ENUM_FLAG_OPERATORS(Window_Style_EX);

    enum class Window_Class_Style : long long
    {
        Byte_Align_Client = CS_BYTEALIGNCLIENT,
        Byte_Align_Window = CS_BYTEALIGNWINDOW,
        Class_DC = CS_CLASSDC,
        DBL_CLKS = CS_DBLCLKS,
        Drop_Shadow = CS_DROPSHADOW,
        Global_Class = CS_GLOBALCLASS,
        H_Redraw = CS_HREDRAW,
        No_Close = CS_NOCLOSE,
        Own_DC = CS_OWNDC,
        Parent_DC = CS_PARENTDC,
        Save_Bits = CS_SAVEBITS,
        V_Redraw = CS_VREDRAW
    };
    DEFINE_ENUM_FLAG_OPERATORS(Window_Class_Style);

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
        
        CreateWindowHelper() = default;

        CreateWindowHelper(TDSTR className, TDSTR windowName, Window_Style style, int posX, int posY, int width, int height, HWND parent, HMENU menu, HINSTANCE instanceHandle, LPVOID param = nullptr)
        {
            lpClassName = className;
            lpWindowName = windowName;
            dwStyle = static_cast<DWORD>(style);
            X = posX;
            Y = posY;
            nWidth = width;
            nHeight = height;
            hWndParent = parent;
            hMenu = menu;
            hInstance = instanceHandle;
            lpParam = param;
        }

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
                Window_Style::Overlapped | Window_Style::Caption | Window_Style::Sys_Menu | Window_Style::Minimize_Box | Window_Style::Maximize_Box,           // Window style
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
                Window_Style::Pop_Up,            // Window style
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

        CreateWindowHelperEX() = default;

        CreateWindowHelperEX(Window_Style_EX styleEX, TDSTR className, TDSTR windowName, Window_Style style, int posX, int posY, int width, int height, HWND parent, HMENU menu, HINSTANCE instanceHandle, LPVOID param = nullptr)
        {
            dwExStyle = static_cast<DWORD>(styleEX);
            lpClassName = className;
            lpWindowName = windowName;
            dwStyle = static_cast<DWORD>(style);
            X = posX;
            Y = posY;
            nWidth = width;
            nHeight = height;
            hWndParent = parent;
            hMenu = menu;
            hInstance = instanceHandle;
            lpParam = param;
        }

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
				Window_Style_EX::None,                           // Optional window styles.
				lpClassName,                     // Window class
				lpClassName,    // Window text
                Window_Style::Overlapped | Window_Style::Caption | Window_Style::Sys_Menu | Window_Style::Minimize_Box | Window_Style::Maximize_Box,          // Window style
				x, y,	// Position
				windowWidth, windowHeight,					// Size
				nullptr,							// Parent window    
                nullptr,							// Menu
				hInstance,					// Instance handle
				nullptr							// Additional application data
			};

			return helper;
		}

        static CreateWindowHelperEX Borderless(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
        {
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

            CreateWindowHelperEX helper =
            {
                Window_Style_EX::None,                           // Optional window styles.
                lpClassName,                     // Window class
                lpClassName,    // Window text
                Window_Style::Pop_Up,              // Window style
                x,y,	// Position
                windowWidth, windowHeight,					// Size
                nullptr,							// Parent window    
                nullptr,							// Menu
                hInstance,					// Instance handle
                nullptr							// Additional application data
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

		wc.style = static_cast<UINT>(Window_Class_Style::H_Redraw | Window_Class_Style::V_Redraw);
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
        wc.style = static_cast<UINT>(Window_Class_Style::H_Redraw | Window_Class_Style::V_Redraw);
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