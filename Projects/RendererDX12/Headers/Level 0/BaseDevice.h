#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class BaseDevice
    {
    public:
        BaseDevice(const D3D_FEATURE_LEVEL& featureLevel, const UINT& adapterID);
        ~BaseDevice();

        inline ID3D12Device2* GetInterface() const noexcept { return m_device.Get(); }
        inline IDXGIAdapter3* GetAdapter() const noexcept { return m_adapter.Get(); }
        inline UINT GetNodeID() const noexcept { return m_nodeID; }
        inline UINT GetNodeMask() const noexcept { return 1 << m_nodeID; }

    private:
        void RegisterNode();
        void UnregisterNode();
    private:
        ComPtr<ID3D12Device2> m_device;
        ComPtr<IDXGIAdapter3> m_adapter;
        UINT m_nodeID = 0;
        static UINT m_registeredNodes;
    };
}