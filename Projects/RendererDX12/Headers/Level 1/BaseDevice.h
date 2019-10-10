#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    namespace LevelOne
    {
        class BaseDevice
        {
        public:
            BaseDevice(D3D_FEATURE_LEVEL featureLevel, UINT adapterID);
            ~BaseDevice();

            inline ID3D12Device2* GetInterface() { return m_device.Get(); }
            inline IDXGIAdapter3* GetAdapter() { return m_adapter.Get(); }
            inline UINT GetNodeID() { return m_nodeID; }
            inline UINT GetNodeMask() { return 1 << m_nodeID; }

        private:
            void CreateNodeMask();
            void DeleteNode();
        private:
            ComPtr<ID3D12Device2> m_device;
            ComPtr<IDXGIAdapter3> m_adapter;
            UINT m_nodeID = 0;
            static UINT m_activeNodes;
        };
    }
}