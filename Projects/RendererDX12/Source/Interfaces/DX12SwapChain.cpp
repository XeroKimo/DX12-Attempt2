#include "RendererDX12.h"
#include "Interfaces/DX12SwapChain.h"

DX12SwapChain::DX12SwapChain()
{
}

void DX12SwapChain::Initialize(DX12Device* device, HWND windowHandle, UINT windowWidth, UINT windowHeight)
{
	m_device = device;
	HRESULT hr;
	ComPtr<IDXGIFactory2> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(factory.GetAddressOf()));
	if (FAILED(hr))
		assert(false);


	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;	//How many samples per pixel are we drawing

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.Width = windowWidth;
	desc.Height = windowHeight;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Stereo = false;
	desc.SampleDesc = sampleDesc;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Scaling = DXGI_SCALING_NONE;

	ComPtr<IDXGISwapChain1> tempSwapChain;

	DX12CommandQueue* queue = device->GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
	hr = factory->CreateSwapChainForHwnd(queue->GetCommandQueue(), windowHandle, &desc, nullptr, nullptr, tempSwapChain.GetAddressOf());
	if (FAILED(hr))
		assert(false);

	hr = tempSwapChain->QueryInterface(m_swapChain.GetAddressOf());	
	if (FAILED(hr))
		assert(false);

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = desc.BufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = device->GetNodeMask();

	hr = device->GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_renderTargetHeap.GetAddressOf()));
	if (FAILED(hr))
		assert(false);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
	m_descriptorHeapSize = m_device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;

	m_frameBuffers.resize(desc.BufferCount);
	for (int i = 0; i < desc.BufferCount; i++)
	{
		hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_frameBuffers[i].GetAddressOf()));
		if (FAILED(hr))
			assert(false);
		
		m_device->GetDevice()->CreateRenderTargetView(m_frameBuffers[i].Get(), &rtvDesc, rtvHandle);
		rtvHandle.ptr += m_descriptorHeapSize;
	}

	m_viewPort.Width = windowWidth;
	m_viewPort.Height = windowHeight;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.MinDepth = 0;
	m_viewPort.MaxDepth = 1;

	m_rect.top = 0;
	m_rect.left = 0;
	m_rect.bottom = windowHeight;
	m_rect.right = windowWidth;
}

void DX12SwapChain::ClearBackBuffer(shared_ptr<DX12CommandList>& commandList)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += m_descriptorHeapSize * m_swapChain->GetCurrentBackBufferIndex();
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };
	commandList->GetCommandList()->OMSetRenderTargets(1, &handle, FALSE, nullptr);
	commandList->GetCommandList()->ClearRenderTargetView(handle, color, 1, &m_rect);
}
