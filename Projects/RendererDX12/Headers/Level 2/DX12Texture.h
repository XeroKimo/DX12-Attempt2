#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class DX12CommandList;
    class DX12Texture
    {
    public:

        void InitializeTexture2D(ID3D12Device* device, DX12CommandList* commandList, std::wstring filename);

        void Set(DX12CommandList* commandList, const UINT& paramIndex);
    private:
        void ParseImage(std::wstring fileName, unique_ptr<BYTE[]>& outImageData, unsigned int& outImageHeight, unsigned int& outImageWidth);

    private:
        ComPtr<ID3D12DescriptorHeap> m_heap;
        ComPtr<ID3D12Resource> m_resource;
        D3D12_SHADER_RESOURCE_VIEW_DESC m_resourceView;
    };
}