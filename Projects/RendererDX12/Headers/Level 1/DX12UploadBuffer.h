#pragma once
#include "DX12Header.h"

#define ConstBufferSize 256

class DX12UploadBuffer
{
public:
	DX12UploadBuffer();

	void Initialize(ID3D12Device* device, UINT creationNodeMask, UINT visibleNodeMask, UINT64 size);	
	void Reset();

	D3D12_GPU_VIRTUAL_ADDRESS UploadCBVSRVUAV(void* data, UINT64 size);
	void UploadData(ID3D12GraphicsCommandList* commandList, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data, UINT64 intermediateOffset, UINT numSubResources, UINT firstSubResource = 0);

	inline bool HasSpace(UINT64 requestedSpace) { return (m_totalSize - requestedSpace - m_usedSize) >= 0; }
	inline ID3D12Resource* GetResource() { return m_resource.GetInterface(); }
private:
	//ComPtr<ID3D12Resource> m_resource;
    DX12Resource m_resource;
	UINT64 m_totalSize;
	UINT64 m_usedSize;
	UINT8* m_mappedAddress;
};