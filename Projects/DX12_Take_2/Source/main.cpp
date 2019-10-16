#include "PCH.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    using namespace RendererDX12;
    using namespace WinApplication;
    using namespace RendererDX12::Helpers;

    Window application;
    PlatformClock clock;
	{
		TDSTR className = L"DX12Renderer";
		WNDCLASS wc = WndClassStandard(hInstance, static_cast<WNDPROC>(WindowProcMain), className);
		CreateWindowHelper helper = CreateWindowHelper::Standard(hInstance, 1280, 720, className);
        helper.ConvertToClientSize();

		if (!application.Initialize(wc, helper))
			return 1;
	}

    BaseDevice device(D3D_FEATURE_LEVEL_11_0, 0);
    if (!device.GetInterface())
        return 1;

    ManagerConstantBuffer managerUploadBuffer(&device, 1000);
    ManagerCommandAllocator commandAllocatorManager(&device, &managerUploadBuffer);
    DeviceCommandModule commandModule(&device, &commandAllocatorManager, 0, 0, 1);

    BaseSwapChain swapChain(device.GetInterface(), device.GetNodeMask(), commandModule.GetCommandQueueInterface(D3D12_COMMAND_LIST_TYPE_DIRECT, 0), application.GetWindowHandle(), application.GetWindowWidth(), application.GetWindowHeight());
    if (!swapChain.GetInterface())
        return 1;

    unique_ptr<PipelineState> pipeline;
	{
		GraphicsPipelineStateDesc pipelineDesc;
		{
            RootDescriptorTable table;
            table.AddTable(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);

			//pipelineDesc.desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

			pipelineDesc.rootSignatureDesc.CreateRootDescriptor(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
			pipelineDesc.rootSignatureDesc.CreateRootDescriptorTable(table);
			pipelineDesc.rootSignatureDesc.AddStaticSampler(Defaults::StaticSamplerAnisotropic(0));

			pipelineDesc.inputLayout.AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0);
			pipelineDesc.inputLayout.AddElement("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
			pipelineDesc.inputLayout.AddElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

			pipelineDesc.vsShader.CompileShaderFromFile(L"Resources/Shaders/VertexShader.hlsl", "vs_5_0");
			pipelineDesc.psShader.CompileShaderFromFile(L"Resources/Shaders/PixelShader.hlsl", "ps_5_0");

			pipelineDesc.GeneratePipelineState(device.GetInterface(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0);
		}

        pipeline = make_unique<PipelineState>(pipelineDesc.pipelineState, pipelineDesc.rootSiganture);
	}

	struct Vertex { Vector3 pos; Vector4 color; Vector2 UV; };
	//Vertex bottomLeft = { Vector3(-0.5f, -0.5f, 0), Vector4(0, 0, 0, 0), Vector2(0.0f, 0.0f) };
	//Vertex bottomRight = { Vector3(0.5f, -0.5f, 0), Vector4(0, 0, 0, 0), Vector2(1.0f, 0.0f) };
	//Vertex topLeft = { Vector3(-0.5f, 0.5f, 0), Vector4(0, 0, 0, 0), Vector2(0.0f, 1.0f) };
	//Vertex topRight = { Vector3(0.5f, 0.5f, 0), Vector4(0, 0, 0, 0), Vector2(1.0f, 1.0f) };

	//Vertex vertices[] =
	//{
	//	bottomLeft,
	//	topLeft,
	//	bottomRight,

	//	topLeft,
	//	topRight,
	//	bottomRight
	//};

    Vertex bottomLeftFront = { Vector3(-0.5f, -0.5f, 0.5f), Vector4(0,0,0,0), Vector2(0.0f,0.0f) };
    Vertex bottomRightFront = { Vector3(0.5f, -0.5f, 0.5f), Vector4(0,0,0,0), Vector2(1.0f,0.0f) };
    Vertex topLeftFront = { Vector3(-0.5f, 0.5f, 0.5f), Vector4(0,0,0,0), Vector2(0.0f,1.0f) };
    Vertex topRightFront = { Vector3(0.5f, 0.5f, 0.5f), Vector4(0,0,0,0), Vector2(1.0f,1.0f) };

    Vertex bottomLeftBack = { Vector3(-0.5f, -0.5f, -0.5f), Vector4(0,0,0,0), Vector2(1.0f,0.0f) };
    Vertex bottomRightBack = { Vector3(0.5f, -0.5f, -0.5f), Vector4(0,0,0,0), Vector2(0.0f,0.0f) };
    Vertex topLeftBack = { Vector3(-0.5f, 0.5f, -0.5f), Vector4(0,0,0,0), Vector2(1.0f,1.0f) };
    Vertex topRightBack = { Vector3(0.5f, 0.5f, -0.5f), Vector4(0,0,0,0), Vector2(0.0f,1.0f) };

    Vertex vertices[] =
{
    bottomLeftFront,
	topLeftFront,
	bottomRightFront,

	topLeftFront,
	topRightFront,
	bottomRightFront,
    //Front square
    bottomRightFront,
    topRightFront,
    bottomRightBack,

    topRightFront,
    topRightBack,
    bottomRightBack,
    //Right Square
    topLeftFront,
    bottomLeftFront,
    bottomLeftBack,

    topLeftBack,
    topLeftFront,
    bottomLeftBack,
    //LeftSquare
    topLeftBack,
    bottomLeftBack,
    bottomRightBack,

    topRightBack,
    topLeftBack,
    bottomRightBack,
    //Back Square
    topLeftFront,
    topLeftBack,
    topRightFront,

    topRightFront,
    topLeftBack,
    topRightBack,
    //Top Square
    bottomLeftBack,
    bottomLeftFront,
    bottomRightFront,

    bottomLeftBack,
    bottomRightFront,
    bottomRightBack,
    //Bottom Square
};

	void* vertexData = reinterpret_cast<void*>(&vertices);

	unique_ptr<CommandList> cl = commandModule.GetCommandList(D3D12_COMMAND_LIST_TYPE_COPY);
	Mesh mesh;
	mesh.CreateVertexBuffer(cl.get(), &vertices, sizeof(Vertex), sizeof(vertices) / sizeof(Vertex));
  
	Texture texture;
	texture.InitializeTexture2D(device.GetInterface(), cl.get(), L"Resources/test.jpg");

    commandModule.ExecuteCommandList(cl,0);
    commandModule.SignalQueue(D3D12_COMMAND_LIST_TYPE_COPY, 0);
    commandModule.StallQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, 0, D3D12_COMMAND_LIST_TYPE_COPY, 0);

	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projMatrix;
	worldMatrix.SetPosition(Vector3(0, 0, -3));
	projMatrix.SetOrtho(16,9, 0.0f, 100);

    Quaternion quat;

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
			clock.Tick();

            std::chrono::milliseconds chronoTime = clock.GetChronoDeltaTime<std::chrono::milliseconds>();
            float time = clock.GetDeltaTime<float>();
            //quat.r = 2.0f;
            //quat.k = 2.0f;
            quat.Rotate(Vector3(1, 1, 1).GetNormalized(), rotateSpeed* clock.GetDeltaTime<float>());
            //quat.Rotate(Vector3(0, 0, 0), rotateSpeed * clock.GetDeltaTime<float>());

            buffer.worldMatrix.Identity();
            buffer.worldMatrix.SetPosition(Vector3(0, 0, -3));
            buffer.worldMatrix *= quat.GetRotation();

			Vector3 angle = buffer.viewMatrix.GetEulerAngles();

			cl = commandModule.GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
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
			commandModule.SyncQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, 0);

            swapChain.GetInterface()->Present(0, 0);
		}
	}

	return 0;
}