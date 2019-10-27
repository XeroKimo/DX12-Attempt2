#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class CommandList;

    struct TextureData
    {
        unique_ptr<unsigned char[]> imageData;
        unsigned int imageWidth = 0;
        unsigned int imageHeight = 0;

        TextureData() = default;
        TextureData(TextureData& other)
        {
            imageData = std::move(other.imageData);
            imageWidth = other.imageWidth;
            imageHeight = other.imageHeight;
        }
    };
    class Texture
    {
    public:

        shared_ptr<UploadBuffer> InitializeTexture2D(const BaseCommandList* const commandList, const TextureData& textureData);

        void SetForDraw(const BaseCommandList* const commandList, const UINT& paramIndex);
        ID3D12DescriptorHeap* GetResourceHeap() const noexcept { return m_heap.Get(); }
        ID3D12Resource* GetResource() const noexcept { return m_resource.Get(); }

    private:
        ComPtr<ID3D12DescriptorHeap> m_heap;
        ComPtr<ID3D12Resource> m_resource;
        D3D12_SHADER_RESOURCE_VIEW_DESC m_resourceView = {};
    };
}