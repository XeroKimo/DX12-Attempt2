#include "PCH.h"

LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    using namespace RendererDX12;
    using namespace WinApplication;
    using namespace RendererDX12::Helpers;

    ModuleManager moduleManager;

    WinApp app;
    Renderer renderer;
    Game game;

    moduleManager.RegisterModule<WinApp>(&app);
    moduleManager.RegisterModule<Renderer>(&renderer);
    moduleManager.RegisterModule<Game>(&game);

    if (!app.Initialize(hInstance, WindowProcMain))
        return 1;
    if (!renderer.Initialize(app.GetWindow()))
        return 1;
    app.Run(&game);

	return 0;
}

LRESULT CALLBACK WindowProcMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WinApplication::WNDProcPassthrough* pWinApp = reinterpret_cast<WinApplication::WNDProcPassthrough*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            if (MessageBox(0, L"Are you sure you want to exit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
                DestroyWindow(hwnd);
        }
        break;
    }
    if (pWinApp)
        return pWinApp->Invoke(hwnd, uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}