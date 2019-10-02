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
		DX12HRootSignatureDesc::DescriptorTable table;
		table.AddTable(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		unique_ptr<DX12HGraphicsPipelineStateDesc> pipelineDesc = make_unique<DX12HGraphicsPipelineStateDesc>();
		pipelineDesc->rootSignatureDesc.CreateRootDescriptor(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		pipelineDesc->rootSignatureDesc.CreateRootDescriptorTable(table);
		pipelineDesc->rootSignatureDesc.AddStaticSampler(sampler);

		pipelineDesc->inputLayout.AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0);
		pipelineDesc->inputLayout.AddElement("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
		pipelineDesc->inputLayout.AddElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

		pipelineDesc->vsShader.CompileShaderFromFile(L"Resources/Shaders/VertexShader.hlsl", "vs_5_0");
		pipelineDesc->psShader.CompileShaderFromFile(L"Resources/Shaders/PixelShader.hlsl", "ps_5_0");

		pipelineDesc->GeneratePipelineState(device, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0);

		pipeline.Initialize(pipelineDesc->pipelineState, pipelineDesc->rootSiganture);
	}
	

	struct Vertex { float x; float y; float z; float r; float g; float b; float a; float u; float v; };
	Vertex bottomLeft = { -0.5f,-0.5f, 1, 0,0,0,0, 0.0f, 0.0f };
	Vertex bottomRight = { 0.5f,-0.5f, 1, 0,0,0,0, 1.0f, 0.0f };
	Vertex topLeft = { -0.5f,0.5f, 1,0,0,0,0, 0.0f, 1.0f };
	Vertex topRight = { 0.5f,0.5f, 1,0,0,0,0, 1.0f, 1.0f};
	//Vertex vertices[] =
	//{
	//	{ 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 1.0f },
	//	{ 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	//	{ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
	//};
	Vertex vertices[] =
	{
		bottomLeft,
		topLeft,
		bottomRight,

		topLeft,
		topRight,
		bottomRight
	};
	void* vertexData = reinterpret_cast<void*>(&vertices);
	unique_ptr<DX12CommandList> cl = renderer.GetCommandList();
	DX12Mesh mesh;
	mesh.CreateVertexBuffer(cl.get(), &vertices, sizeof(Vertex), sizeof(vertices) / sizeof(Vertex));
  
	DX12Texture texture;
	texture.InitializeTexture2D(renderer.GetDeviceInterface()->GetBase()->GetInterface(), cl.get(), L"Resources/Maquia.png");

	renderer.ExecuteCommandList(cl);
    renderer.SignalCommandQueue();
    renderer.SyncCommandQueue();
	renderer.ResetCommandQueue();

	Vector3 pos(1, 0, 0);

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
			cl = renderer.GetCommandList();
			ID3D12GraphicsCommandList* commandList = cl->GetBase()->GetInterface();
			renderer.GetSwapChain()->ClearBackBuffer(commandList);
			pipeline.SetPipelineState(commandList);
			pipeline.SetRootSignature(commandList);
			commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//cl->SetConstantBuffer(0, &pos, sizeof(Vector3));
			mesh.Set(cl.get());
			texture.Set(cl.get(), 1);
			mesh.Draw(cl.get());
			renderer.SubmitCommandList(cl);
			renderer.ExecuteWaitingCommandLists();
			renderer.Present();
		}
	}

	return 0;
}