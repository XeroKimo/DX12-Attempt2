#include "PCH.h"
#include <iostream>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	WinApp application;
	if (!application.Initialize(hInstance, 1280, 720))
		return 1;

	DX12BasicInterface renderer;
	if (!renderer.Initialize(application.GetHandle(), application.GetWindowWidth(), application.GetWindowHeight()))
		return 1;

	PlatformClock clock;

	ID3D12Device* device = renderer.GetDeviceInterface()->GetBase()->GetInterface();

	DX12PipelineState pipeline;
	{
		unique_ptr<DX12HGraphicsPipelineStateDesc> pipelineDesc = make_unique<DX12HGraphicsPipelineStateDesc>();
		pipelineDesc->rootSignatureDesc.CreateRootDescriptor(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		pipelineDesc->inputLayout.AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0);
		pipelineDesc->inputLayout.AddElement("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
		pipelineDesc->vsShader.CompileShaderFromFile(L"Resources/Shaders/VertexShader.hlsl", "vs_5_0");
		pipelineDesc->psShader.CompileShaderFromFile(L"Resources/Shaders/PixelShader.hlsl", "ps_5_0");
		pipelineDesc->GeneratePipelineState(device, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0);

		pipeline.Initialize(pipelineDesc->pipelineState, pipelineDesc->rootSiganture);
	}
	

	struct Vertex { float x; float y; float z; float r; float g; float b; float a; };
	Vertex vertices[] =
	{
		{ 0.0f, 0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f },
		{ 0.5f, -0.5f, 0.0f,0.0f,0.0f,1.0f,1.0f },
		{ -0.5f, -0.5f, 0.0f,1.0f,0.0f,0.0f,1.0f },
	};
	void* vertexData = reinterpret_cast<void*>(&vertices);
	shared_ptr<DX12CommandList> cl = renderer.GetCommandList();
	DX12Mesh mesh;
	mesh.CreateVertexBuffer(cl, &vertices, sizeof(Vertex), 3);
  
	renderer.ExecuteCommandList(cl);
    renderer.SignalCommandQueue();
    renderer.SyncCommandQueue();
	renderer.ResetCommandQueue();


	struct Vector3 { float x; float y; float z; };
	Vector3 pos = { 1.0f, 0.0f,0.0f };

	MSG msg;
	while (application.IsRunning())
	{
		if (application.PeekMsg(msg))
		{
			switch (msg.message)
			{
			case WM_QUIT:
				application.Quit();
				break;

			default:
				application.ReadMessage(msg);
			}

		}
		else
		{
			clock.Update();

			cl = renderer.GetDeviceInterface()->GetCommandList();
			ID3D12GraphicsCommandList* commandList = cl->GetBase()->GetInterface();
			renderer.GetSwapChain()->ClearBackBuffer(commandList);
			pipeline.SetPipelineState(commandList);
			pipeline.SetRootSignature(commandList);
			commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			mesh.Set(cl);
			mesh.Draw(cl);
			renderer.ExecuteCommandList(cl);
			renderer.Present();
		}
	}

	return 0;
}