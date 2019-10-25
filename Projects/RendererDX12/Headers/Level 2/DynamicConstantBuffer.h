#pragma once
#include "Level 1/UploadBuffer.h"


namespace RendererDX12
{
    class DynamicConstantBuffer
    {
    private:
        UploadBuffer m_uploadBuffer;
        UINT8* m_mappedAddress = nullptr;

        UINT64 m_totalSize = 0;
        UINT64 m_usedSize = 0;
        static const int ConstBufferAlignment = 256;
    public:
        DynamicConstantBuffer(BaseDevice* device, UINT64 size);

        D3D12_GPU_VIRTUAL_ADDRESS Upload(void* data, UINT64 size);
        void Reset();
        inline bool HasSpace(UINT64 requestedSpace) { return (m_totalSize - requestedSpace - m_usedSize) >= 0; }
    };
}