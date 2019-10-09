#include "RendererDX12.h"
#include "Level 1/DX12BaseSwapChain.h"

namespace RendererDX12
{
    DX12BaseSwapChain::DX12BaseSwapChain(ID3D12Device* device, UINT nodeMask, ID3D12CommandQueue* commandQueue, HWND windowHandle, UINT windowWidth, UINT windowHeight)
    {
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
        rtvHeapDesc.NodeMask = nodeMask;

        hr = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_renderTargetHeap.GetAddressOf()));
        assert(SUCCEEDED(hr));

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
        m_descriptorHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

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

            device->CreateRenderTargetView(m_frameBuffers[i].Get(), &rtvDesc, rtvHandle);
            rtvHandle.ptr += m_descriptorHeapSize;
        }

        m_viewPort.Width = static_cast<float>(windowWidth);
        m_viewPort.Height = static_cast<float>(windowHeight);
        m_viewPort.TopLeftX = 0;
        m_viewPort.TopLeftY = 0;
        m_viewPort.MinDepth = 0;
        m_viewPort.MaxDepth = 0;

        m_rect.top = 0;
        m_rect.left = 0;
        m_rect.bottom = windowHeight;
        m_rect.right = windowWidth;
    }

    void DX12BaseSwapChain::ClearBackBuffer(ID3D12GraphicsCommandList* commandList)
    {
        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_renderTargetHeap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += (static_cast<size_t>(m_descriptorHeapSize)* static_cast<size_t>(m_swapChain->GetCurrentBackBufferIndex()));
        //float color[4] = { 0.1f,0.0f,0.6f,1.0f };
        float color[4] = { 0.5f,0.5f,0.5f,1.0f };
        commandList->RSSetViewports(1, &m_viewPort);
        commandList->RSSetScissorRects(1, &m_rect);

        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_frameBuffers[m_swapChain->GetCurrentBackBufferIndex()].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        commandList->ResourceBarrier(1, &barrier);

        commandList->ClearRenderTargetView(handle, color, 0, nullptr);
        commandList->OMSetRenderTargets(1, &handle, FALSE, nullptr);
    }

    void DX12BaseSwapChain::ReadyBackBuffer(ID3D12GraphicsCommandList* commandList)
    {
        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_frameBuffers[m_swapChain->GetCurrentBackBufferIndex()].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        commandList->ResourceBarrier(1, &barrier);
    }
}