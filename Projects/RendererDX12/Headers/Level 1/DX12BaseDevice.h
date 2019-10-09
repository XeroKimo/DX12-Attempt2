#pragma once
#include "DX12Header.h"

class DX12BaseDevice
{
public:
	DX12BaseDevice(D3D_FEATURE_LEVEL featureLevel, UINT adapterID);
    ~DX12BaseDevice();

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