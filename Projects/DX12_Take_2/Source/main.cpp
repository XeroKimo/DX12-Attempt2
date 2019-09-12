#include "PCH.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	WinApp application;
	if (!application.Initialize(hInstance, 1280, 720))
		return 1;

	DX12Renderer renderer;
	if (!renderer.Initialize(application.GetHandle(), application.GetWindowWidth(), application.GetWindowHeight()))
		return 1;

	PlatformClock clock;

    DX12HInputLayout layout;
    layout.AddElement("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);


    ID3D12Device* device = renderer.GetDeviceInterface()->GetDevice()->GetDevice();

	DX12HRootSignatureDesc desc;
	HRESULT hr = desc.SerializeSignature(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0);
	assert(SUCCEEDED(hr));

	ComPtr<ID3D12RootSignature> rootSignature;
	hr = device->CreateRootSignature(renderer.GetDeviceInterface()->GetDevice()->GetNodeMask(), desc.GetSerializedSignature()->GetBufferPointer(), desc.GetSerializedSignature()->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
	assert(SUCCEEDED(hr));


	DX12HShaderByteCode vsShader;
	vsShader.CompileShaderFromFile(L"Resources/Shaders/VertexShader.hlsl", "vs_5_0");

    DX12HShaderByteCode psShader;
    psShader.CompileShaderFromFile(L"Resources/Shaders/PixelShader.hlsl", "ps_5_0");

    DX12HGraphicsPipelineStateDesc pipelineDesc;
    pipelineDesc.SetShaders(&vsShader.GetByteCode(), &psShader.GetByteCode());

    pipelineDesc.desc.pRootSignature = rootSignature.Get();
    pipelineDesc.desc.InputLayout = layout.GetLayout();

    ComPtr<ID3D12PipelineState> pipelineState;
	hr = device->CreateGraphicsPipelineState(&pipelineDesc.desc, IID_PPV_ARGS(pipelineState.GetAddressOf()));
	assert(SUCCEEDED(hr));

    struct Vertex { float x; float y; float z; };
    Vertex vertices[] =
    {
        { 0.0f, 0.5f, 0.5f },
        { 0.5f, -0.5f, 0.5f },
        { -0.5f, -0.5f, 0.5f },
    };

    ComPtr<ID3D12Resource> vertexResource;
    ComPtr<ID3D12Resource> copyBuffer;

    D3D12_HEAP_PROPERTIES heapProperties = {};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperties.CreationNodeMask = device->GetNodeCount();
    heapProperties.VisibleNodeMask = heapProperties.CreationNodeMask;
    heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_HEAP_PROPERTIES copyProperties = heapProperties;
    copyProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    resourceDesc.Width = sizeof(vertices);
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.Height = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(vertexResource.GetAddressOf()));
    assert(SUCCEEDED(hr));

    hr = device->CreateCommittedResource(&copyProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(copyBuffer.GetAddressOf()));
    assert(SUCCEEDED(hr));

    shared_ptr<DX12CommandList> cl = renderer.GetDeviceInterface()->GetCommandList();
    D3D12_RANGE range = { 0, sizeof(vertices) };
    BYTE* address;
    hr= copyBuffer->Map(0, nullptr, nullptr);
    assert(SUCCEEDED(hr));

    D3D12_SUBRESOURCE_DATA data;
    data.pData = &vertices;
    data.RowPitch = sizeof(vertices);
    data.SlicePitch = sizeof(vertices);


    cl->GetCommandList()->CopyResource(vertexResource.Get(), copyBuffer.Get());
    DX12CommandList::CloseAndExecute(cl);
    renderer.GetDeviceInterface()->SignalAllQueues();
    renderer.GetDeviceInterface()->SyncAllQueues();
    renderer.GetDeviceInterface()->GetCommandQueue()->ResetFenceValue();
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
			cl = renderer.GetDeviceInterface()->GetCommandList();
            cl->GetCommandList()->SetPipelineState(pipelineState.Get());
			renderer.GetSwapChain()->ClearBackBuffer(cl);
			////cl->Close();
			//DX12CommandList::Close(cl);
			DX12CommandList::CloseAndExecute(cl);
			renderer.Present();
		}
	}

	return 0;
}