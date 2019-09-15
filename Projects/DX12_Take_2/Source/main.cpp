#include "PCH.h"
#include <iostream>
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
    layout.AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT,0);
	layout.AddElement("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);


    ID3D12Device* device = renderer.GetDeviceInterface()->GetDevice()->GetDevice();

	DX12HRootSignatureDesc desc;
	desc.CreateRootDescriptor(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	//desc.CreateRootConstant(3, 0);
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

	struct Vertex { float x; float y; float z; float r; float g; float b; float a; };
	Vertex vertices[] =
	{
		{ 0.0f, 0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f },
		{ 0.5f, -0.5f, 0.0f,0.0f,0.0f,1.0f,1.0f },
		{ -0.5f, -0.5f, 0.0f,1.0f,0.0f,0.0f,1.0f },
	};
	void* vertexData = reinterpret_cast<void*>(&vertices);
	shared_ptr<DX12CommandList> cl = renderer.GetDeviceInterface()->GetCommandList();
	DX12Mesh mesh;
	mesh.CreateVertexBuffer(cl.get(), &vertices, sizeof(Vertex), 3);
  
    DX12CommandList::CloseAndExecute(cl);
    renderer.GetDeviceInterface()->SignalAllQueues();
    renderer.GetDeviceInterface()->SyncAllQueues();
    renderer.GetDeviceInterface()->GetCommandQueue()->GetBaseCommandQueue()->ResetFenceValue();


	struct Vector3 { float x; float y; float z; };
	Vector3 pos = { 1.0f, 0.0f,0.0f };
	D3D12_GPU_VIRTUAL_ADDRESS address;

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
			renderer.GetSwapChain()->ClearBackBuffer(cl->GetCommandList());
			cl->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
			cl->GetCommandList()->SetPipelineState(pipelineState.Get());
			address = cl->GetCommandAllocator()->UploadCBVSRVUAV(&pos, sizeof(pos));
			//cl->GetCommandList()->SetGraphicsRoot32BitConstants(0, 3, &pos, 0);
			cl->GetCommandList()->SetGraphicsRootConstantBufferView(0, address);
			cl->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			mesh.Set(cl);
			mesh.Draw(cl);
			//cl->GetCommandList()->IASetVertexBuffers(0, 1, &vBufferView);
			//cl->GetCommandList()->DrawInstanced(3, 1, 0, 0);
			////cl->Close();
			//DX12CommandList::Close(cl);
			DX12CommandList::CloseAndExecute(cl);
			renderer.Present();
		}
	}

	return 0;
}