#include "RendererDX12.h"
#include "Level 2/DX12ManagerConstBuffer.h"

namespace RendererDX12
{
    using namespace LevelOne;
    DX12ManagerConstBuffer::DX12ManagerConstBuffer(BaseDevice* device, UINT64 constBufferSize) :
        m_device(device),
        m_bufferSize((constBufferSize + (1 << 16) - 1) & ~((1 << 16) - 1))
    {
    }

    void DX12ManagerConstBuffer::ResetBuffers(std::vector<unique_ptr<UploadBuffer>>& buffers)
    {
        std::move(buffers.begin(), buffers.end(), std::back_inserter(m_inactiveBuffers));
    }

    unique_ptr<UploadBuffer> DX12ManagerConstBuffer::GetConstBuffer()
    {
        if (m_inactiveBuffers.empty())
            return CreateBuffer(m_bufferSize);
        unique_ptr<UploadBuffer> buffer = std::move(m_inactiveBuffers.back());
        m_inactiveBuffers.pop_back();
        buffer->Reset();
        return buffer;
    }

    unique_ptr<UploadBuffer> DX12ManagerConstBuffer::CreateBuffer(UINT64 size)
    {
        return make_unique<UploadBuffer>(m_device->GetInterface(), m_device->GetNodeMask(), size);
    }
}