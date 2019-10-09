#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    namespace DX12HResource
    {
        inline D3D12_HEAP_PROPERTIES HeapDefault(UINT creationNodeMask, UINT visibleNodeMask = 0)
        {
            D3D12_HEAP_PROPERTIES properties;
            properties.Type = D3D12_HEAP_TYPE_DEFAULT;
            properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            properties.CreationNodeMask = creationNodeMask;
            properties.VisibleNodeMask = (visibleNodeMask == 0) ? creationNodeMask : visibleNodeMask;

            return properties;
        }

        inline D3D12_HEAP_PROPERTIES HeapReadBack(UINT creationNodeMask, UINT visibleNodeMask = 0)
        {
            D3D12_HEAP_PROPERTIES properties;
            properties.Type = D3D12_HEAP_TYPE_READBACK;
            properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            properties.CreationNodeMask = creationNodeMask;
            properties.VisibleNodeMask = (visibleNodeMask == 0) ? creationNodeMask : visibleNodeMask;

            return properties;
        }

        inline D3D12_HEAP_PROPERTIES HeapUpload(UINT creationNodeMask)
        {
            D3D12_HEAP_PROPERTIES properties;
            properties.Type = D3D12_HEAP_TYPE_UPLOAD;
            properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            properties.CreationNodeMask = creationNodeMask;
            properties.VisibleNodeMask = creationNodeMask;

            return properties;
        }

        inline D3D12_HEAP_PROPERTIES HeapCustom(D3D12_CPU_PAGE_PROPERTY pageProperty, D3D12_MEMORY_POOL poolPreference, UINT creationNodeMask, UINT visibleNodeMask = 0)
        {
            D3D12_HEAP_PROPERTIES properties;
            properties.Type = D3D12_HEAP_TYPE_CUSTOM;
            properties.CPUPageProperty = pageProperty;
            properties.MemoryPoolPreference = poolPreference;
            properties.CreationNodeMask = creationNodeMask;
            properties.VisibleNodeMask = (visibleNodeMask == 0) ? creationNodeMask : visibleNodeMask;

            return properties;
        }

        inline D3D12_RESOURCE_DESC Buffer(UINT64 size, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE)
        {
            const UINT64 alignment = ((1 << 16) - 1);

            D3D12_RESOURCE_DESC resourceDesc;
            resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            resourceDesc.Alignment = alignment + 1;
            resourceDesc.Width = size;
            resourceDesc.Height = 1;
            resourceDesc.DepthOrArraySize = 1;
            resourceDesc.MipLevels = 1;
            resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
            resourceDesc.SampleDesc.Count = 1;
            resourceDesc.SampleDesc.Quality = 0;
            resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            resourceDesc.Flags = flags;

            return resourceDesc;
        }

        inline D3D12_RESOURCE_DESC Texture1D(UINT64 width, DXGI_FORMAT format, UINT16 mipLevels, UINT sampleCount = 1, UINT sampleQuality = 0, UINT16 arraySize = 1, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE)
        {
            const UINT64 alignment = (sampleCount == 1) ? ((1 << 16) - 1) : ((1 << 22) - 1);

            D3D12_RESOURCE_DESC resourceDesc;
            resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
            resourceDesc.Alignment = alignment + 1;
            resourceDesc.Width = width;
            resourceDesc.Height = 1;
            resourceDesc.DepthOrArraySize = arraySize;
            resourceDesc.MipLevels = mipLevels;
            resourceDesc.Format = format;
            resourceDesc.SampleDesc.Count = sampleCount;
            resourceDesc.SampleDesc.Quality = sampleQuality;
            resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            resourceDesc.Flags = flags;
            return resourceDesc;
        }

        inline D3D12_RESOURCE_DESC Texture2D(UINT64 width, UINT height, DXGI_FORMAT format, UINT16 mipLevels, UINT sampleCount = 1, UINT sampleQuality = 0, UINT16 arraySize = 1, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE)
        {
            const UINT64 alignment = (sampleCount == 1) ? ((1 << 16) - 1) : ((1 << 22) - 1);

            D3D12_RESOURCE_DESC resourceDesc;
            resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            resourceDesc.Alignment = alignment + 1;
            resourceDesc.Width = width;
            resourceDesc.Height = height;
            resourceDesc.DepthOrArraySize = arraySize;
            resourceDesc.MipLevels = mipLevels;
            resourceDesc.Format = format;
            resourceDesc.SampleDesc.Count = sampleCount;
            resourceDesc.SampleDesc.Quality = sampleQuality;
            resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            resourceDesc.Flags = flags;
            return resourceDesc;
        }

        inline D3D12_RESOURCE_DESC Texture3D(UINT64 width, UINT height, UINT16 depth, DXGI_FORMAT format, UINT16 mipLevels, UINT sampleCount = 1, UINT sampleQuality = 0, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE)
        {
            const UINT64 alignment = (sampleCount == 1) ? ((1 << 16) - 1) : ((1 << 22) - 1);

            D3D12_RESOURCE_DESC resourceDesc;
            resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
            resourceDesc.Alignment = alignment + 1;
            resourceDesc.Width = width;
            resourceDesc.Height = height;
            resourceDesc.DepthOrArraySize = depth;
            resourceDesc.MipLevels = mipLevels;
            resourceDesc.Format = format;
            resourceDesc.SampleDesc.Count = sampleCount;
            resourceDesc.SampleDesc.Quality = sampleQuality;
            resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            resourceDesc.Flags = flags;
            return resourceDesc;
        }
    }
}