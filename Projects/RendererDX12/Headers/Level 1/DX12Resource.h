#pragma once
#include "DX12Header.h"

class DX12Resource
{
public:
    DX12Resource();
    void InitAsBuffer(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask = 0, UINT visibleNodeMask = 0, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE);
    void InitAsTexture1D(ID3D12Device* device, UINT64 width, UINT16 arraySize, UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask = 0, UINT visibleNodeMask = 0, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE);
    void InitAsTexture2D(ID3D12Device* device, UINT64 width, UINT height, UINT16 arraySize, UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask = 0, UINT visibleNodeMask = 0, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE);
    void InitAsTexture3D(ID3D12Device* device, UINT64 width, UINT height, UINT16 depth, UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask = 0, UINT visibleNodeMask = 0, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE);
    void InitAsReadback();
    void InitAsUpload(ID3D12Device* device, UINT64 size, UINT creationNodeMask = 0, UINT visibleNodeMask = 0, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE);
    void InitAsCustom(ID3D12Device* device, D3D12_HEAP_PROPERTIES heapProperties, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask = 0, UINT visibleNodeMask = 0, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE );


    void GetCopyableFootprints();

    inline ID3D12Resource* GetInterface() { return m_resource.Get(); }

private:
    D3D12_HEAP_PROPERTIES CreateHeapDesc(D3D12_HEAP_TYPE type, UINT creationNodeMask, UINT visibleNodeMask, D3D12_CPU_PAGE_PROPERTY pageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL poolPreference = D3D12_MEMORY_POOL_UNKNOWN);
    D3D12_RESOURCE_DESC CreateResourceDesc(D3D12_RESOURCE_DIMENSION dimension, UINT64 alignment, UINT64 width, UINT height, UINT16 depthOrArraySize, UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_TEXTURE_LAYOUT layout, D3D12_RESOURCE_FLAGS flags);
private:
    ComPtr<ID3D12Resource> m_resource;
};