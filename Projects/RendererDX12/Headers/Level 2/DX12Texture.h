#pragma once
#include "DX12Header.h"

class DX12CommandList;
class DX12Texture
{
public:

	void InitializeTexture2D(ID3D12Device* device, DX12CommandList* commandList, std::wstring filename);
private:
	void ParseImage(std::wstring fileName, unique_ptr<BYTE[]>& outImageData, unsigned int& outImageHeight, unsigned int& outImageWidth);

private:
	ComPtr<ID3D12DescriptorHeap> m_heap;
	ComPtr<ID3D12Resource> m_resource;
};