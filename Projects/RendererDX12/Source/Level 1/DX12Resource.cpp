#include "RendererDX12.h"
#include "Level 1/DX12Resource.h"

DX12Resource::DX12Resource()
{
}

void DX12Resource::InitAsBuffer(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask, UINT visibleNodeMask, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resourceFlags)
{
    D3D12_HEAP_PROPERTIES heapProperties = CreateHeapDesc(D3D12_HEAP_TYPE_DEFAULT, creationNodeMask, visibleNodeMask);

    D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc
    (
        D3D12_RESOURCE_DIMENSION_BUFFER,
        1 << 16,                                //Alignment
        size, 1, 1,                             //Width / Height / Depth or Array Size
        1,                                      //Mip levels
        DXGI_FORMAT_UNKNOWN,
        1, 0,                                   //Sample Count / Quality
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        resourceFlags
    );

    HRESULT hr = device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, initialState, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
    assert(SUCCEEDED(hr));
}

void DX12Resource::InitAsTexture1D(ID3D12Device* device, UINT64 width, UINT16 arraySize, UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask, UINT visibleNodeMask, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resourceFlags)
{
    D3D12_HEAP_PROPERTIES heapProperties = CreateHeapDesc(D3D12_HEAP_TYPE_DEFAULT, creationNodeMask, visibleNodeMask);

    D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc
    (
        D3D12_RESOURCE_DIMENSION_TEXTURE1D,
        (sampleCount == 1) ? 1 << 16 : 1 << 22, //Alignment
        width, 1, arraySize,                    //Width / Height / Depth or Array Size
        mipLevels,                              //Mip levels
        format,
        sampleCount, sampleQuality,             //Sample Count / Quality
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        resourceFlags
    );

    HRESULT hr = device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, initialState, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
    assert(SUCCEEDED(hr));
}

void DX12Resource::InitAsTexture2D(ID3D12Device* device, UINT64 width, UINT height, UINT16 arraySize, UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask, UINT visibleNodeMask, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resourceFlags)
{
    D3D12_HEAP_PROPERTIES heapProperties = CreateHeapDesc(D3D12_HEAP_TYPE_DEFAULT, creationNodeMask, visibleNodeMask);

    D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc
    (
        D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        (sampleCount == 1) ? 1 << 16 : 1 << 22, //Alignment
        width, height, arraySize,               //Width / Height / Depth or Array Size
        mipLevels,                              //Mip levels
        format,
        sampleCount, sampleQuality,             //Sample Count / Quality
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        resourceFlags
    );

    HRESULT hr = device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, initialState, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
    assert(SUCCEEDED(hr));
}

void DX12Resource::InitAsTexture3D(ID3D12Device* device, UINT64 width, UINT height, UINT16 depth, UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask, UINT visibleNodeMask, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resourceFlags)
{
    D3D12_HEAP_PROPERTIES heapProperties = CreateHeapDesc(D3D12_HEAP_TYPE_DEFAULT, creationNodeMask, visibleNodeMask);

    D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc
    (
        D3D12_RESOURCE_DIMENSION_TEXTURE3D,
        (sampleCount == 1) ? 1 << 16 : 1 << 22, //Alignment
        width, height, depth,                   //Width / Height / Depth or Array Size
        mipLevels,                              //Mip levels
        format,
        sampleCount, sampleQuality,             //Sample Count / Quality
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        resourceFlags
    );

    HRESULT hr = device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, initialState, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
    assert(SUCCEEDED(hr));
}

void DX12Resource::InitAsUpload(ID3D12Device* device, UINT64 size, UINT creationNodeMask, UINT visibleNodeMask, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resourceFlags)
{
    D3D12_HEAP_PROPERTIES heapProperties = CreateHeapDesc(D3D12_HEAP_TYPE_UPLOAD, creationNodeMask, visibleNodeMask);

    D3D12_RESOURCE_DESC resourceDesc = CreateResourceDesc
    (
        D3D12_RESOURCE_DIMENSION_BUFFER,
        1 << 16,                                //Alignment
        size, 1, 1,                             //Width / Height / Depth or Array Size
        1,                                      //Mip levels
        DXGI_FORMAT_UNKNOWN,
        1, 0,                                   //Sample Count / Quality
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        resourceFlags
    );

    HRESULT hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES | heapFlags, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
    assert(SUCCEEDED(hr));
}

void DX12Resource::InitAsCustom(ID3D12Device* device, D3D12_HEAP_PROPERTIES heapProperties, D3D12_RESOURCE_DESC resourceDesc, D3D12_RESOURCE_STATES initialState, UINT creationNodeMask, UINT visibleNodeMask, D3D12_HEAP_FLAGS heapFlags)
{
    HRESULT hr = device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, initialState, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
    assert(SUCCEEDED(hr));
}

D3D12_HEAP_PROPERTIES DX12Resource::CreateHeapDesc(D3D12_HEAP_TYPE type, UINT creationNodeMask, UINT visibleNodeMask, D3D12_CPU_PAGE_PROPERTY pageProperty, D3D12_MEMORY_POOL poolPreference)
{
    D3D12_HEAP_PROPERTIES properties = {};
    properties.Type = type;
    properties.CPUPageProperty = pageProperty;
    properties.MemoryPoolPreference = poolPreference;
    properties.CreationNodeMask = creationNodeMask;
    properties.VisibleNodeMask = visibleNodeMask;
    return properties;
}

D3D12_RESOURCE_DESC DX12Resource::CreateResourceDesc(D3D12_RESOURCE_DIMENSION dimension, UINT64 alignment, UINT64 width, UINT height, UINT16 depthOrArraySize, UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_TEXTURE_LAYOUT layout, D3D12_RESOURCE_FLAGS flags)
{
    D3D12_RESOURCE_DESC desc;
    desc.Dimension = dimension;
    desc.Alignment = alignment;
    desc.Width = width;
    desc.Height = height;
    desc.DepthOrArraySize = depthOrArraySize;
    desc.MipLevels = mipLevels;
    desc.Format = format;
    desc.SampleDesc.Count = sampleCount;
    desc.SampleDesc.Quality = sampleQuality;
    desc.Layout = layout;
    desc.Flags = flags;
    return desc;
}