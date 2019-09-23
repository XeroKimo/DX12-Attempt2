#include "RendererDX12.h"
#include "Level 1/DX12BaseDevice.h"

DX12BaseDevice::DX12BaseDevice() :
    m_nodeMask(0)
{
}


void DX12BaseDevice::Initialize(D3D_FEATURE_LEVEL featureLevel, UINT adapterID)
{

	HRESULT hr;
	ComPtr<IDXGIFactory2> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(factory.GetAddressOf()));
	assert(SUCCEEDED(hr));


	ComPtr<IDXGIAdapter1> adapter;
	hr = factory->EnumAdapters1(adapterID, adapter.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = adapter->QueryInterface(m_adapter.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = D3D12CreateDevice(m_adapter.Get(), featureLevel, IID_PPV_ARGS(m_device.GetAddressOf()));
	assert(SUCCEEDED(hr));

    m_nodeMask = m_device->GetNodeCount();
}
