#include "PCH.h"
#include "Modules/Game.h"

Game::Game()
{
}

void Game::Initialize()
{
    m_eventManager = m_moduleManager->GetModule<WinApp>()->GetApplication()->eventManager.get();
    m_moduleManager->GetModule<WinApp>()->GetWindow()->passThrough.SubscribeEvent(WM_DESTROY, Delegates::Delegate<void(WPARAM, LPARAM)>::Generate<Game, &Game::OnWindowDestory>(this));
    m_moduleManager->GetModule<WinApp>()->GetWindow()->passThrough.SubscribeEvent(WM_KEYDOWN, Delegates::Delegate<void(WPARAM, LPARAM)>::Generate<Game, &Game::RotateCamera>(this));
    m_moduleManager->GetModule<WinApp>()->GetWindow()->passThrough.SubscribeEvent(WM_KEYUP, Delegates::Delegate<void(WPARAM, LPARAM)>::Generate<Game, &Game::StopRotate>(this));

    m_eventManager->RegisterEventDispatcher<EventGame>();
    m_eventManager->RegisterListener<EventGame>(Delegates::Delegate<void(EventGame*)>::Generate<Game, &Game::OnEvent>(this));
    m_eventManager->RecordEvent<EventGame>(std::make_unique<EventGame>());

    CreateDefaults();
}

void Game::Update(float deltaTime)
{
    if (aPressed)
    {
        buffer3.worldMatrix.RotateZ(-90.0f * deltaTime);
    }    
    if (dPressed)
    {
        buffer3.worldMatrix.RotateZ(90.0f * deltaTime);
    }
    if (wPressed)
    {
        buffer3.worldMatrix.RotateX(90.0f * deltaTime);
    }
    if (sPressed)
    {
        buffer3.worldMatrix.RotateX(-90.0f * deltaTime);
    }
    Quaternion quat;
    size_t size = sizeof(Vector4);
    const float amountToRotate = 360.f / 180.f;
    //float rotateSpeed = 60.0f;
    //quat.Rotate(Vector3(1, 1, 1).GetNormalized(), rotateSpeed * deltaTime);
    quat *= Quaternion(1 - deltaTime * 3, -deltaTime, -deltaTime, -deltaTime);
    //quat *= Quaternion(1 - amountToRotate * 3, -amountToRotate, -amountToRotate, -amountToRotate);
    quat.Normalize();

    //buffer.worldMatrix.Identity();
    //buffer.worldMatrix.SetPosition(Vector3(-2, 0, -3));
    buffer.worldMatrix *= quat.GetRotation();

    quat.Identity();
    quat *= Quaternion(1 - deltaTime * 3, deltaTime, deltaTime, deltaTime);
    //quat *= Quaternion(1 - amountToRotate * 3, amountToRotate, amountToRotate, amountToRotate);
    quat.Normalize();

    //buffer2.worldMatrix.Identity();
    //buffer2.worldMatrix.SetPosition(Vector3(2, 0, -3));
    buffer2.worldMatrix *= quat.GetRotation();

	//quat.Identity();
	//quat *= Quaternion(1 - deltaTime * 0.5, deltaTime / 2, 0, 0);
	//quat.Normalize();
	//buffer3.worldMatrix *= quat.GetRotation();


    //buffer2.worldMatrix.RotateX(30 * deltaTime);

}

void Game::Draw()
{
    using namespace RendererDX12;
    
    DeviceCommandModule* commandModule = m_moduleManager->GetModule<Renderer>()->GetDeviceCommandModule();
    BaseSwapChain* swapChain = m_moduleManager->GetModule<Renderer>()->GetSwapChain();

    unique_ptr<CommandList> cl = commandModule->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);

    swapChain->ClearBackBuffer(cl->GetBase());

	m_pipelineState->SetPipelineState(cl->GetBase());
    m_pipelineState->SetGraphicsRootSignature(cl->GetBase());

    m_texture.SetForDraw(cl->GetBase(), 1);
    m_mesh.SetForDraw(cl->GetBase());

	cl->SetConstantBuffer(0, &buffer, sizeof(buffer));
	m_mesh.DrawIndexedInstance(cl->GetBase());
    cl->SetConstantBuffer(0, &buffer2, sizeof(buffer2));
    m_mesh.DrawIndexedInstance(cl->GetBase());

	m_houseTexture.SetForDraw(cl->GetBase(), 1);
	m_house.SetForDraw(cl->GetBase());

	cl->SetConstantBuffer(0, &buffer3, sizeof(buffer3));
	m_house.DrawIndexedInstance(cl->GetBase());
	

    swapChain->ReadyBackBuffer(cl->GetBase());

    commandModule->ExecuteCommandList(cl, 0);
    commandModule->SignalAllQueues();
	commandModule->SyncQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, 0);

    swapChain->GetInterface()->Present(0, 0);
}

void Game::OnModuleRegisterChanged(ModuleManager* moduleManager)
{
    m_moduleManager = moduleManager;
}

void Game::OnEvent(EventGame* event)
{
    int i = 2;
}

void Game::OnWindowDestory(WPARAM wParam, LPARAM lParam)
{
    int i = 0;
}

void Game::RotateCamera(WPARAM wParam, LPARAM lParam)
{
    if (wParam == 'A')
    {
        aPressed = true;
    }
    if (wParam == 'D')
        dPressed = true;
    if (wParam == 'S')
        sPressed = true;
    if (wParam == 'W')
        wPressed = true;
}
void Game::StopRotate(WPARAM wParam, LPARAM lParam)
{
    if (wParam == 'A')
    {
        aPressed = false;
    }
    if (wParam == 'D')
        dPressed = false;
    if (wParam == 'S')
        sPressed = false;
    if (wParam == 'W')
        wPressed = false;
}
void Game::CreateDefaults()
{
    using namespace RendererDX12;
    using namespace RendererDX12::Helpers;

    Renderer* renderer = m_moduleManager->GetModule<Renderer>();
    BaseDevice* device = renderer->GetDevice();
    DeviceCommandModule* commandModule = renderer->GetDeviceCommandModule();
    {
        GraphicsPipelineStateDesc pipelineDesc;
        {
            RootDescriptorTable table;
            table.AddTable(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);

            //pipelineDesc.desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			pipelineDesc.desc.RasterizerState.FrontCounterClockwise = true;

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

    Vertex bottomLeftFront = { Vector3(-0.5f, -0.5f, 0.5f), Vector4(0,0,0,0), Vector2(0.0f,0.0f) }; //0
    Vertex bottomRightFront = { Vector3(0.5f, -0.5f, 0.5f), Vector4(0,0,0,0), Vector2(1.0f,0.0f) }; //1
    Vertex topLeftFront = { Vector3(-0.5f, 0.5f, 0.5f), Vector4(0,0,0,0), Vector2(0.0f,1.0f) };     //2
    Vertex topRightFront = { Vector3(0.5f, 0.5f, 0.5f), Vector4(0,0,0,0), Vector2(1.0f,1.0f) };     //3

    Vertex bottomLeftBack = { Vector3(-0.5f, -0.5f, -0.5f), Vector4(0,0,0,0), Vector2(1.0f,0.0f) }; //4
    Vertex bottomRightBack = { Vector3(0.5f, -0.5f, -0.5f), Vector4(0,0,0,0), Vector2(0.0f,0.0f) }; //5
    Vertex topLeftBack = { Vector3(-0.5f, 0.5f, -0.5f), Vector4(0,0,0,0), Vector2(1.0f,1.0f) };     //6
    Vertex topRightBack = { Vector3(0.5f, 0.5f, -0.5f), Vector4(0,0,0,0), Vector2(0.0f,1.0f) };     //7

    Vertex verts[] =
    {
        bottomLeftFront,
        bottomRightFront,
        topLeftFront,
        topRightFront,

        bottomLeftBack,
        bottomRightBack,
        topLeftBack,
        topRightBack
    };

    UINT indices[] =
    {
        0, 2, 1,
        2, 3, 1,
        //Front square Done
        1, 3, 5,
        3, 7, 5,
        //Right Square Done
        2, 0, 4,
        6, 2, 4,
        //LeftSquare Done
        6, 4, 5,
        7, 6, 5,
        //Back Square Done
        2, 6, 3,
        3, 6, 7,
        //Top Square Done
        4, 0, 1,
        4, 1, 5,
        //Bottom Square
    };

    //Vertex vertices[] =
    //{
    //    bottomLeftFront,    //0
	   // topLeftFront,       //2
	   // bottomRightFront,   //1

	   // topLeftFront,       //2
	   // topRightFront,      //3
	   // bottomRightFront,   //1
    //    //Front square
    //    bottomRightFront,   //1
    //    topRightFront,      //3
    //    bottomRightBack,    //5

    //    topRightFront,      //3
    //    topRightBack,       //7
    //    bottomRightBack,    //5
    //    //Right Square
    //    topLeftFront,       //2
    //    bottomLeftFront,    //0
    //    bottomLeftBack,     //4

    //    topLeftBack,        //6
    //    topLeftFront,       //2
    //    bottomLeftBack,     //4
    //    //LeftSquare
    //    topLeftBack,        //6
    //    bottomLeftBack,     //4
    //    bottomRightBack,    //5

    //    topRightBack,       //7
    //    topLeftBack,        //6
    //    bottomRightBack,    //5
    //    //Back Square
    //    topLeftFront,       //2
    //    topLeftBack,        //6
    //    topRightFront,      //3

    //    topRightFront,      //3
    //    topLeftBack,        //6
    //    topRightBack,       //7
    //    //Top Square
    //    bottomLeftBack,     //4
    //    bottomLeftFront,    //0
    //    bottomRightFront,   //1

    //    bottomLeftBack,     //4
    //    bottomRightFront,   //1
    //    bottomRightBack,    //5
    //    //Bottom Square
    //};


	FbxNode* fbx = FBXLoader::LoadFBX("Resources/houseA/house");
	FBXLoader::LoadMeshData data = FBXLoader::LoadMesh("Resources/houseA/house",fbx);
	std::vector<Vertex> fbxVerts;
	for (int i = 0; i < data.vertices.size(); i++)
	{
		Vertex vert;
		vert.pos = Vector3(data.vertices[i].pos.x, data.vertices[i].pos.y, data.vertices[i].pos.z);
		vert.UV = Vector2(data.vertices[i].uv.x, data.vertices[i].uv.y);
		fbxVerts.push_back(vert);
	}

	void* vertexData = static_cast<void*>(&verts);

	unique_ptr<CommandList> cl = commandModule->GetCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);

    cl->UploadData(m_mesh.CreateVertexBuffer(cl->GetBase(), &verts, sizeof(Vertex), sizeof(verts) / sizeof(Vertex)));
    cl->UploadData(m_mesh.CreateIndexBuffer(cl->GetBase(), indices, sizeof(indices) / sizeof(UINT)));

	cl->UploadData(m_house.CreateVertexBuffer(cl->GetBase(), fbxVerts.data(), sizeof(Vertex), fbxVerts.size()));
	cl->UploadData(m_house.CreateIndexBuffer(cl->GetBase(), data.indices.data(), data.indices.size()));

    RendererDX12::TextureData textureData;
    ParseImage(L"Resources/test.jpg", textureData.imageData, textureData.imageHeight, textureData.imageWidth);

    cl->UploadData(m_texture.InitializeTexture2D(cl->GetBase(), textureData));

	ParseImage(L"Resources/houseA/houseA.jpg", textureData.imageData, textureData.imageHeight, textureData.imageWidth);
	cl->UploadData(m_houseTexture.InitializeTexture2D(cl->GetBase(), textureData));

    commandModule->ExecuteCommandList(cl,0);
    commandModule->SignalQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, 0);
    commandModule->SyncQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, 0);
    //commandModule->SignalQueue(D3D12_COMMAND_LIST_TYPE_COPY, 0);
    //commandModule->StallQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, 0, D3D12_COMMAND_LIST_TYPE_COPY, 0);

	Matrix4x4 worldMatrix;
	Matrix4x4 projMatrix;
	worldMatrix.SetPosition(Vector3(-2, 0, -3));
	projMatrix.SetPerspective(90,16.f/9.f, 0.0f, 1000);
	//projMatrix.SetOrtho(16.f, 9.f, 0.0f, 100);

	//struct cBuffer { Matrix4x4 worldMatrix; Matrix4x4 viewMatrix; Matrix4x4 projMatrix; };
	buffer = { worldMatrix, camera, projMatrix };
    worldMatrix.SetPosition(Vector3(2, 0, -3));
    buffer2 = { worldMatrix, camera, projMatrix };
	worldMatrix.Identity();
	worldMatrix.SetPosition(Vector3(0, -20, -100));
	worldMatrix.RotateX(90);
	buffer3 = { worldMatrix, camera, projMatrix };
}