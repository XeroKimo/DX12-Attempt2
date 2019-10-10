#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class UploadBuffer
    {
    public:
        UploadBuffer(ID3D12Device* device, UINT nodeMask, UINT64 size);
        UploadBuffer(ID3D12GraphicsCommandList* commandList, UINT nodeMask, ID3D12Resource* targetResource, D3D12_SUBRESOURCE_DATA* data);

        void Reset();

        D3D12_GPU_VIRTUAL_ADDRESS UploadCBVSRVUAV(void* data, UINT64 size);

        inline bool HasSpace(UINT64 requestedSpace) { return (m_totalSize - requestedSpace - m_usedSize) >= 0; }
        inline ID3D12Resource* GetInterface() { return m_resource.Get(); }
    private:
        static const int ConstBufferSize = 256;
        ComPtr<ID3D12Resource> m_resource;
        UINT64 m_totalSize = 0;
        UINT64 m_usedSize = 0;
        UINT8* m_mappedAddress = nullptr;
    };
}