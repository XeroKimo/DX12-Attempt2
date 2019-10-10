#pragma once
#include "DX12Header.h"
#include "Level 1/UploadBuffer.h"

namespace RendererDX12
{
    class DX12ManagerConstBuffer
    {
    public:
        DX12ManagerConstBuffer(LevelOne::BaseDevice* device, UINT64 constBufferSize);
        void ResetBuffers(std::vector<unique_ptr<LevelOne::UploadBuffer>>& buffers);

        unique_ptr<LevelOne::UploadBuffer> GetConstBuffer();
    private:
        unique_ptr<LevelOne::UploadBuffer> CreateBuffer(UINT64 size);
    private:
        LevelOne::BaseDevice* m_device;
        std::vector<unique_ptr<LevelOne::UploadBuffer>> m_inactiveBuffers;
        UINT64 m_bufferSize;
    };
}