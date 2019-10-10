#include "RendererDX12.h"

namespace RendererDX12
{
    namespace LevelOne
    {
        UINT BaseDevice::m_activeNodes = 0;
        BaseDevice::BaseDevice(D3D_FEATURE_LEVEL featureLevel, UINT adapterID)
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

        BaseDevice::~BaseDevice()
        {
            DeleteNode();
        }

        void BaseDevice::CreateNodeMask()
        {
            for (UINT i = 0; i < 32; i++)
            {
                UINT checkMask = 1 << i;
                if (checkMask & (~m_activeNodes))
                {
                    m_nodeID = i;
                    m_activeNodes |= checkMask;
                    break;
                }
            }
        }

        void BaseDevice::DeleteNode()
        {
            m_activeNodes ^= GetNodeMask();
        }
    }
}