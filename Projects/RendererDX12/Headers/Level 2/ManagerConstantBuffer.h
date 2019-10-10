#pragma once
#include "DX12Header.h"
#include "Level 1/UploadBuffer.h"

namespace RendererDX12
{
    class ManagerConstantBuffer
    {
    public:
        ManagerConstantBuffer(BaseDevice* device, UINT64 constBufferSize);
        void ResetBuffers(std::vector<unique_ptr< UploadBuffer>>& buffers);

        unique_ptr< UploadBuffer> GetConstBuffer();
    private:
        unique_ptr< UploadBuffer> CreateBuffer(UINT64 size);
    private:
        BaseDevice* m_device;
        std::vector<unique_ptr< UploadBuffer>> m_inactiveBuffers;
        UINT64 m_bufferSize;
    };
}