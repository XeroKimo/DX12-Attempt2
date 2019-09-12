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

	DX12HRootSignatureDesc desc;
	desc.CreateRootConstant(1, 0);
	HRESULT hr = desc.SerializeSignature(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0);
	assert(SUCCEEDED(hr));

	ComPtr<ID3D12RootSignature> rootSignature;
	hr = renderer.GetDevice()->GetDevice()->CreateRootSignature(renderer.GetDevice()->GetNodeMask(), desc.GetSerializedSignature()->GetBufferPointer(), desc.GetSerializedSignature()->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
	assert(SUCCEEDED(hr));


	DX12HShaderByteCode vsShader;
	vsShader.CompileShaderFromFile(L"Resources/Shaders/VertexShader.hlsl", "vs_5_0");

	DX12HInputLayout layout;
	layout.AddElement("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);

	D3D12_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterDesc.ForcedSampleCount = 0;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0;

	ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.InputLayout = layout.GetLayout();
	gpsDesc.pRootSignature = rootSignature.Get();
	gpsDesc.VS = vsShader.GetByteCode();
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleDesc.Quality = 0;
	gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpsDesc.RasterizerState = rasterDesc;
	gpsDesc.NumRenderTargets = 1;


	hr = renderer.GetDevice()->GetDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pipelineState.GetAddressOf()));
	assert(SUCCEEDED(hr));

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
			//cl->Close();
			//DX12CommandList::Close(cl);
			DX12CommandList::CloseAndExecuteAll(cl);
			renderer.Present();
		}
	}

	return 0;
}