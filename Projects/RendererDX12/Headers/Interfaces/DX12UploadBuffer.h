#pragma once
#include "DX12Header.h"

class DX12UploadBuffer
{
public:
	DX12UploadBuffer();

	void Initialize(ID3D12Device* device, UINT size);

	inline ID3D12Resource* GetResource() { return m_resource.Get(); }
private:
	ComPtr<ID3D12Resource> m_resource;
};