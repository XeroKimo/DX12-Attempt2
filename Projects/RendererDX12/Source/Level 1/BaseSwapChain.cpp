#include "RendererDX12.h"

namespace RendererDX12
{
    BaseSwapChain::BaseSwapChain(BaseDevice* const device, ID3D12CommandQueue* const commandQueue, const HWND& windowHandle, const UINT& windowWidth, const UINT& windowHeight)
    {
        m_device = device;
        HRESULT hr;
        ComPtr<IDXGIFactory2> factory;
        hr = CreateDXGIFactory1(IID_PPV_ARGS(factory.GetAddressOf()));
        assert(SUCCEEDED(hr));

        DXGI_SAMPLE_DESC sampleDesc = {};
        sampleDesc.Count = 1;	//How many samples per pixel are we drawing
        sampleDesc.Quality = 0;

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
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        ComPtr<IDXGISwapChain1> tempSwapChain;




        hr = factory->CreateSwapChainForHwnd(commandQueue, windowHandle, &desc, nullptr, nullptr, tempSwapChain.GetAddressOf());
        assert(SUCCEEDED(hr));
        if (FAILED(hr))
            return;

        hr = tempSwapChain->QueryInterface(m_swapChain.GetAddressOf());
        assert(SUCCEEDED(hr));

        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
        rtvHeapDesc.NumDescriptors = desc.BufferCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvHeapDesc.NodeMask = device->GetNodeMask();

        hr = device->GetInterface()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_renderTargetHeap.GetAddressOf()));
        assert(SUCCEEDED(hr));

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
        m_descriptorHeapSize = device->GetInterface()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = desc.Format;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;
        rtvDesc.Texture2D.PlaneSlice = 0;

        m_frameBuffers.resize(desc.BufferCount);
        for (UINT i = 0; i < desc.BufferCount; i++)
        {
            hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_frameBuffers[i].GetAddressOf()));
            assert(SUCCEEDED(hr));

            device->GetInterface()->CreateRenderTargetView(m_frameBuffers[i].Get(), &rtvDesc, rtvHandle);
            rtvHandle.ptr += m_descriptorHeapSize;
        }

        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsvHeapDesc.NodeMask = device->GetNodeMask();

        device->GetInterface()->CreateCommittedResource
        (
            &Helpers::HeapDefault(device->GetNodeMask()), 
            D3D12_HEAP_FLAG_NONE, 
            &Helpers::ResourceTexture2D(windowWidth, windowHeight, DXGI_FORMAT_R24G8_TYPELESS, 0,1,0,1, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
            D3D12_RESOURCE_STATE_DEPTH_WRITE, 
            nullptr, 
            IID_PPV_ARGS(m_depthStencil.GetAddressOf())
        );

        hr = device->GetInterface()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_depthStencilHeap.GetAddressOf()));
        assert(SUCCEEDED(hr));       
        
        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilView;
        depthStencilView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilView.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilView.Flags = D3D12_DSV_FLAG_NONE;
        depthStencilView.Texture2D.MipSlice = 0;

        device->GetInterface()->CreateDepthStencilView(m_depthStencil.Get(), &depthStencilView, m_depthStencilHeap->GetCPUDescriptorHandleForHeapStart());

        m_viewPort.Width = static_cast<float>(windowWidth);
        m_viewPort.Height = static_cast<float>(windowHeight);
        m_viewPort.TopLeftX = 0;
        m_viewPort.TopLeftY = 0;
        m_viewPort.MinDepth = 0;
        m_viewPort.MaxDepth = 1.0f;

        m_rect.top = 0;
        m_rect.left = 0;
        m_rect.bottom = windowHeight;
        m_rect.right = windowWidth;
    }

    void BaseSwapChain::ClearBackBuffer(const BaseCommandList* const commandList) const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += (static_cast<size_t>(m_descriptorHeapSize) * static_cast<size_t>(m_swapChain->GetCurrentBackBufferIndex()));
        //float color[4] = { 0.1f,0.0f,0.6f,1.0f };
        float color[4] = { 0.5f,0.5f,0.5f,1.0f };
        commandList->GetInterface()->RSSetViewports(1, &m_viewPort);
        commandList->GetInterface()->RSSetScissorRects(1, &m_rect);

        commandList->GetInterface()->ResourceBarrier(1, &Helpers::ResourceBarrierTransition(m_frameBuffers[m_swapChain->GetCurrentBackBufferIndex()].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

        commandList->GetInterface()->ClearRenderTargetView(handle, color, 0, nullptr);
        commandList->GetInterface()->ClearDepthStencilView(m_depthStencilHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 1, &m_rect);
        commandList->GetInterface()->OMSetRenderTargets(1, &handle, FALSE, &m_depthStencilHeap->GetCPUDescriptorHandleForHeapStart());
    }

    void BaseSwapChain::ReadyBackBuffer(const BaseCommandList* const commandList) const
    {
        commandList->GetInterface()->ResourceBarrier(1, &Helpers::ResourceBarrierTransition(m_frameBuffers[m_swapChain->GetCurrentBackBufferIndex()].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
    }

    void BaseSwapChain::ResizeSwapChain()
    {
        for (auto& frameBuffer : m_frameBuffers)
        {
            frameBuffer.ReleaseAndGetAddressOf();
        }

        //m_swapChain->SetFullscreenState(true, nullptr);

        m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();

        DXGI_SWAP_CHAIN_DESC1 desc;
        m_swapChain->GetDesc1(&desc);

        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = desc.Format;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;
        rtvDesc.Texture2D.PlaneSlice = 0;

        m_frameBuffers.resize(desc.BufferCount);
        for (UINT i = 0; i < desc.BufferCount; i++)
        {
            HRESULT hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_frameBuffers[i].GetAddressOf()));
            assert(SUCCEEDED(hr));

            m_device->GetInterface()->CreateRenderTargetView(m_frameBuffers[i].Get(), &rtvDesc, rtvHandle);
            rtvHandle.ptr += m_descriptorHeapSize;
        }

        m_depthStencil.ReleaseAndGetAddressOf();


        m_device->GetInterface()->CreateCommittedResource
        (
            &Helpers::HeapDefault(m_device->GetNodeMask()),
            D3D12_HEAP_FLAG_NONE,
            &Helpers::ResourceTexture2D(desc.Width, desc.Height, DXGI_FORMAT_D24_UNORM_S8_UINT, 0, 1, 0, 1, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            nullptr,
            IID_PPV_ARGS(m_depthStencil.GetAddressOf())
        );

        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilView;
        depthStencilView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilView.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilView.Flags = D3D12_DSV_FLAG_NONE;
        depthStencilView.Texture2D.MipSlice = 0;

        m_device->GetInterface()->CreateDepthStencilView(m_depthStencil.Get(), &depthStencilView, m_depthStencilHeap->GetCPUDescriptorHandleForHeapStart());


        m_viewPort.Width = static_cast<float>(desc.Width);
        m_viewPort.Height = static_cast<float>(desc.Height);

        m_rect.bottom = desc.Height;
        m_rect.right = desc.Width;
    }
}