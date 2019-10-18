#include "PCH.h"

LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    using namespace RendererDX12;
    using namespace WinApplication;
    using namespace RendererDX12::Helpers;

    ModuleManager moduleManager;
    Renderer renderer;
    Game game;
    WinApp app;

    moduleManager.RegisterModule(&renderer);
    moduleManager.RegisterModule(&game);
    moduleManager.RegisterModule(&app);

    if (!app.Initialize(hInstance, WindowProcMain))
        return 1;
    if (!renderer.Initialize(app.GetWindow()))
        return 1;
    app.Run(&game);

	return 0;
}

LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //WinApp* pWinApp = reinterpret_cast<WinApp*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (uMsg)
    {
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