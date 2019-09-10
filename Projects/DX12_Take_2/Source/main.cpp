#include "PCH.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WinApp application;
	if (!application.Initialize(hInstance, 1280, 720))
		return 1;

	DX12Renderer renderer;
	if (!renderer.Initialize(application.GetHandle(), application.GetWindowWidth(), application.GetWindowHeight()))
		return 1;

	//GameInstance game(&application, &renderer);
	PlatformClock clock;

	MSG msg;
	while (application.IsRunning())
	{
		if (application.PeekMsg(msg))
		{
			if (msg.message == WM_QUIT)
				application.Quit();
		}
		else
		{
			clock.Update();
			shared_ptr<DX12CommandList> cl = renderer.GetDevice()->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
			renderer.GetSwapChain()->ClearBackBuffer(cl);
			cl->Close();
			renderer.Present();
		}
	}

	return 0;
}
