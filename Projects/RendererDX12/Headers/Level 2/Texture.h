#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class CommandList;

    struct TextureData
    {
        TextureData() = default;
        TextureData(TextureData& other)
        {
            imageData = std::move(other.imageData);
            imageWidth = other.imageWidth;
            imageHeight = other.imageHeight;
        }
        unique_ptr<unsigned char[]> imageData;
        unsigned int imageWidth;
        unsigned int imageHeight;
    };
    class Texture
    {
    public:

        void InitializeTexture2D(ID3D12Device* device, CommandList* commandList, TextureData textureData);

        void Set(CommandList* commandList, const UINT& paramIndex);
        ID3D12DescriptorHeap* GetResourceHeap() { return m_heap.Get(); }
        ID3D12Resource* GetResource() { return m_resource.Get(); }

    private:
        ComPtr<ID3D12DescriptorHeap> m_heap;
        ComPtr<ID3D12Resource> m_resource;
        D3D12_SHADER_RESOURCE_VIEW_DESC m_resourceView = {};
    };
}