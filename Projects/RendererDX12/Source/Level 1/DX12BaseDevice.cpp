#include "RendererDX12.h"
#include "Level 1/DX12BaseDevice.h"


UINT DX12BaseDevice::activeNodes = 0;
DX12BaseDevice::DX12BaseDevice() :
    m_nodeMask(0)
{
}

DX12BaseDevice::~DX12BaseDevice()
{
    DeleteNode();
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

    CreateNodeMask();
}

void DX12BaseDevice::CreateNodeMask()
{
    for (UINT i = 0; i < 32; i++)
    {
        UINT checkMask = 1 << i;
        if (checkMask & (~activeNodes))
        {
            m_nodeMask = checkMask;
            activeNodes |= checkMask;
            break;
        }
    }

}

void DX12BaseDevice::DeleteNode()
{
    activeNodes ^= m_nodeMask;
}
