#include "RendererDX12.h"

DX12Renderer::DX12Renderer()
{
}

bool DX12Renderer::Initialize(HWND windowHandle, UINT windowWidth, UINT windowHeight)
{
	m_allocatorManager = make_unique<DX12CommandAllocatorManager>();
	m_device = make_unique<DX12Device>();

	m_device->Initialize(m_allocatorManager.get());
	m_allocatorManager->Initialize(m_device.get());

	m_swapChain = make_unique<DX12SwapChain>();
	m_swapChain->Initialize(m_device.get(), windowHandle, windowWidth, windowHeight);

    ComPtr<ID3DBlob> rootSignatureBlob;
    ComPtr<ID3DBlob> error;
    DX12HRootSignatureDesc desc;
    desc.CreateRootConstant(1, 0);
    HRESULT hr = desc.GetSerializedRootSignature(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0, rootSignatureBlob.GetAddressOf(), error.GetAddressOf());
    assert(SUCCEEDED(hr));

    ComPtr<ID3D12RootSignature> rootSignature;
    hr = m_device->GetDevice()->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
    assert(SUCCEEDED(hr));

    ComPtr<ID3D12PipelineState> pipelineState;

	return true;
}

void DX12Renderer::Present()
{
	m_device->ExecuteCommandListManager();
	m_device->GetCommandQueue()->SignalGPU();
	m_device->GetCommandQueue()->SyncQueue(INFINITE);

	m_swapChain->GetSwapChain()->Present(0, 0);
	m_device->GetCommandQueue()->ResetFenceValue();
}
