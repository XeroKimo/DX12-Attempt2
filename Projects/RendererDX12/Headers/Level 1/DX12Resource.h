#pragma once
#include "DX12Header.h"

class DX12Resource
{
public:
    DX12Resource();
    void InitAsBuffer(ID3D12Device* device, UINT creationNodeMask, UINT visibleNodeMask, UINT64 size);
    void InitAsTexture1D(ID3D12Device* device, UINT creationNodeMask, UINT visibleNodeMask, UINT64 size);
    void InitAsTexture2D(ID3D12Device* device, UINT creationNodeMask, UINT visibleNodeMask, UINT64 size);
    void InitAsTexture3D(ID3D12Device* device, UINT creationNodeMask, UINT visibleNodeMask, UINT64 size);
    void InitAsReadback();
    void InitAsUpload(ID3D12Device* device, UINT creationNodeMask, UINT visibleNodeMask, UINT64 size);
    void InitAsCustom();


    ID3D12Resource* GetInterface() { return m_resource.Get(); }
private:
    ComPtr<ID3D12Resource> m_resource;
};