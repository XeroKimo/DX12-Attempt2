#include "PCH.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WinApp application;
	if (!application.Initialize(hInstance, 1280, 720))
		return 1;

	DX12Renderer renderer;
	if (!renderer.Initialize(application.GetHandle(), application.GetWindowWidth(), application.GetWindowHeight()))
		return 1;


	std::vector<int> test{ 1,2,3,4 };
	std::vector<int> copied;

	std::copy(test.begin(), test.end(), std::back_inserter(copied));

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
			shared_ptr<DX12CommandList> cl = renderer.GetDevice()->GetCommandList();
			renderer.GetSwapChain()->ClearBackBuffer(cl);
			DX12CommandList::CloseAndExecuteAll(cl);
			renderer.Present();
		}
	}

	return 0;
}