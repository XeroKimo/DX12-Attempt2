#pragma once
#include "DX12Header.h"
#include "Level 2/DynamicConstantBuffer.h"

namespace RendererDX12
{
    class ManagerConstantBuffer
    {
    public:
        ManagerConstantBuffer(BaseDevice* device, UINT64 constBufferSize);
        void ResetBuffers(std::vector<unique_ptr<DynamicConstantBuffer>>& buffers);

        unique_ptr<DynamicConstantBuffer> GetConstBuffer();
    private:
        unique_ptr<DynamicConstantBuffer> CreateBuffer(UINT64 size);
    private:
        BaseDevice* m_device;
        std::vector<unique_ptr<DynamicConstantBuffer>> m_inactiveBuffers;
        UINT64 m_bufferSize;
    };
}