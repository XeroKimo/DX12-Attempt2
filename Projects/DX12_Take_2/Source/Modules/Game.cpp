#include "PCH.h"
#include "Modules/Game.h"

Game::Game()
{
}

void Game::OnEvent(WinApplication::IEvent* pEvent)
{
}

void Game::Initialize()
{
    m_eventManager = m_moduleManager->GetModule<WinApp, ModuleType::Application>()->GetApplication()->eventManager.get();
    //RegisterListener()
    CreateDefaults();
}

void Game::Update(float deltaTime)
{
    Quaternion quat;

    float rotateSpeed = 60.0f;
    quat.Rotate(Vector3(1, 1, 1).GetNormalized(), rotateSpeed * deltaTime);

    buffer.worldMatrix *= quat.GetRotation();
}

void Game::Draw()
{
    using namespace RendererDX12;
    
    DeviceCommandModule* commandModule = m_moduleManager->GetModule<Renderer, ModuleType::Renderer>()->GetDeviceCommandModule();
    BaseSwapChain* swapChain = m_moduleManager->GetModule<Renderer, ModuleType::Renderer>()->GetSwapChain();

    unique_ptr<CommandList> cl = commandModule->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
	ID3D12GraphicsCommandList* commandList = cl->GetBase()->GetInterface();

    swapChain->ClearBackBuffer(commandList);

	m_pipelineState->SetPipelineState(commandList);
    m_pipelineState->SetGraphicsRootSignature(commandList);
	commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cl->SetConstantBuffer(0, &buffer, sizeof(buffer));
    m_texture.Set(cl.get(), 1);
	m_mesh.Set(cl.get());
	m_mesh.Draw(cl.get());

    swapChain->ReadyBackBuffer(commandList);

    commandModule->ExecuteCommandList(cl, 0);
    commandModule->SignalAllQueues();
    //renderer.SyncAllQueues();
	commandModule->SyncQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, 0);

    swapChain->GetInterface()->Present(0, 0);
}

void Game::OnModuleRegisterChanged(ModuleManager* moduleManager)
{
    m_moduleManager = moduleManager;
}

const ModuleType Game::GetModuleType()
{
    return ModuleType::Game;
}

void Game::CreateDefaults()
{
    using namespace RendererDX12;
    using namespace RendererDX12::Helpers;

    Renderer* renderer = m_moduleManager->GetModule<Renderer, ModuleType::Renderer>();
    BaseDevice* device = renderer->GetDevice();
    DeviceCommandModule* commandModule = renderer->GetDeviceCommandModule();
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

			pipelineDesc.GeneratePipelineState(device->GetInterface(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0);
		}

        m_pipelineState = make_unique<PipelineState>(pipelineDesc.pipelineState, pipelineDesc.rootSiganture);
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

	unique_ptr<CommandList> cl = commandModule->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);

	m_mesh.CreateVertexBuffer(cl.get(), &vertices, sizeof(Vertex), sizeof(vertices) / sizeof(Vertex));

	m_texture.InitializeTexture2D(device->GetInterface(), cl.get(), L"Resources/test.jpg");

    commandModule->ExecuteCommandList(cl,0);
    //commandModule->SignalQueue(D3D12_COMMAND_LIST_TYPE_COPY, 0);
    //commandModule->StallQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, 0, D3D12_COMMAND_LIST_TYPE_COPY, 0);

	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projMatrix;
	worldMatrix.SetPosition(Vector3(0, 0, -3));
	projMatrix.SetOrtho(16,9, 0.0f, 100);

	//struct cBuffer { Matrix4x4 worldMatrix; Matrix4x4 viewMatrix; Matrix4x4 projMatrix; };
	buffer = { worldMatrix, viewMatrix, projMatrix };
}
