#include "PCH.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    using namespace RendererDX12;
    using namespace WinApplication;

    Window application;
    PlatformClock clock;
	{
		TDSTR className = L"DX12Renderer";
		WNDCLASS wc = WndClassStandard(hInstance, static_cast<WNDPROC>(WindowProc), className);
		CreateWindowHelper helper = CreateWindowHelper::Standard(hInstance, 1280, 720, className);
        helper.ConvertToClientSize();

		if (!application.Initialize(wc, helper))
			return 1;
	}

    DX12BaseDevice device(D3D_FEATURE_LEVEL_11_0, 0);
    if (!device.GetInterface())
        return 1;

    DX12ManagerConstBuffer managerUploadBuffer(&device, 1000);
    DX12ManagerCommandAllocator commandAllocatorManager(&device, &managerUploadBuffer);
    DX12DeviceCommandModule commandModule(&device, &commandAllocatorManager, 1, 0, 1);

    DX12BaseSwapChain swapChain(device.GetInterface(), device.GetNodeMask(), commandModule.GetCommandQueueInterface(Command_List_Type::Direct, 0), application.GetHandle(), application.GetWindowWidth(), application.GetWindowHeight());
    if (!swapChain.GetInterface())
        return 1;

    unique_ptr<DX12PipelineState> pipeline;
	{
		DX12HGraphicsPipelineStateDesc pipelineDesc;
		{
            DX12HRootSignatureDesc::DescriptorTable table;
            table.AddTable(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);

            D3D12_STATIC_SAMPLER_DESC sampler = {};
            sampler.Filter = D3D12_FILTER_ANISOTROPIC;
            sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.MipLODBias = 0;
            sampler.MaxAnisotropy = 16;
            sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            sampler.MinLOD = 0.0f;
            sampler.MaxLOD = D3D12_FLOAT32_MAX;
            sampler.ShaderRegister = 0;
            sampler.RegisterSpace = 0;
            sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

			pipelineDesc.desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

			pipelineDesc.rootSignatureDesc.CreateRootDescriptor(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
			pipelineDesc.rootSignatureDesc.CreateRootDescriptorTable(table);
			pipelineDesc.rootSignatureDesc.AddStaticSampler(sampler);

			pipelineDesc.inputLayout.AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0);
			pipelineDesc.inputLayout.AddElement("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
			pipelineDesc.inputLayout.AddElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

			pipelineDesc.vsShader.CompileShaderFromFile(L"Resources/Shaders/VertexShader.hlsl", "vs_5_0");
			pipelineDesc.psShader.CompileShaderFromFile(L"Resources/Shaders/PixelShader.hlsl", "ps_5_0");

			pipelineDesc.GeneratePipelineState(device.GetInterface(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0);
		}

        pipeline = make_unique<DX12PipelineState>(pipelineDesc.pipelineState, pipelineDesc.rootSiganture);
	}

	struct Vertex { Vector3 pos; Vector4 color; Vector2 UV; };
	Vertex bottomLeft = { Vector3(-0.5f, -0.5f, 0), Vector4(0, 0, 0, 0), Vector2(0.0f, 0.0f) };
	Vertex bottomRight = { Vector3(0.5f, -0.5f, 0), Vector4(0, 0, 0, 0), Vector2(1.0f, 0.0f) };
	Vertex topLeft = { Vector3(-0.5f, 0.5f, 0), Vector4(0, 0, 0, 0), Vector2(0.0f, 1.0f) };
	Vertex topRight = { Vector3(0.5f, 0.5f, 0), Vector4(0, 0, 0, 0), Vector2(1.0f, 1.0f) };

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

	unique_ptr<DX12CommandList> cl = commandModule.GetCommandList(Command_List_Type::Copy);
	DX12Mesh mesh;
	mesh.CreateVertexBuffer(cl.get(), &vertices, sizeof(Vertex), sizeof(vertices) / sizeof(Vertex));
  
	DX12Texture texture;
	texture.InitializeTexture2D(device.GetInterface(), cl.get(), L"Resources/test.jpg");

    commandModule.ExecuteCommandList(cl,0);
    commandModule.SignalQueue(Command_List_Type::Copy, 0);
    commandModule.StallQueue(Command_List_Type::Direct, 0, Command_List_Type::Copy, 0);

	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projMatrix;
	worldMatrix.SetPosition(Vector3(0, 0, -3));
	projMatrix.SetOrtho(3,3, 0.0f, 100);
	//projMatrix.SetPerspective(90, 1280/720, 0.f, 1000);

	float rotateSpeed = 60.0f;

	struct cBuffer { Matrix4x4 worldMatrix; Matrix4x4 viewMatrix; Matrix4x4 projMatrix; };

	cBuffer buffer = { worldMatrix, viewMatrix, projMatrix };
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
				application.ReadMsg(msg);
			}

		}
		else
		{
			clock.Update();

			//buffer.viewMatrix.RotateX(rotateSpeed/2 * clock.GetDeltaTime());
			//buffer.worldMatrix.Translate(Vector3(0, 0, -0.1f)* clock.GetDeltaTime());
			//buffer.worldMatrix.RotateY(rotateSpeed * clock.GetDeltaTime());
			buffer.viewMatrix.RotateZ(rotateSpeed * static_cast<float>(clock.GetDeltaTime()));

			Vector3 angle = buffer.worldMatrix.GetEulerAngles();

			cl = commandModule.GetCommandList(Command_List_Type::Direct);
			ID3D12GraphicsCommandList* commandList = cl->GetBase()->GetInterface();

            swapChain.ClearBackBuffer(commandList);

			pipeline->SetPipelineState(commandList);
			pipeline->SetGraphicsRootSignature(commandList);
			commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			cl->SetConstantBuffer(0, &buffer, sizeof(buffer));
            texture.Set(cl.get(), 1);
			mesh.Set(cl.get());
			mesh.Draw(cl.get());

            swapChain.ReadyBackBuffer(commandList);

            commandModule.ExecuteCommandList(cl, 0);
            commandModule.SignalAllQueues();
            //renderer.SyncAllQueues();
			commandModule.SyncQueue(Command_List_Type::Direct, 0);

            swapChain.GetInterface()->Present(0, 0);
		}
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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