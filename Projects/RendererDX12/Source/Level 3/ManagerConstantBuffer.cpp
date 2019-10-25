#include "RendererDX12.h"

namespace RendererDX12
{
    ManagerConstantBuffer::ManagerConstantBuffer(BaseDevice* device, UINT64 constBufferSize) :
        m_device(device),
        m_bufferSize((constBufferSize + (1 << 16) - 1) & ~((1 << 16) - 1))
    {
    }

    void ManagerConstantBuffer::ResetBuffers(std::vector<unique_ptr<DynamicConstantBuffer>>& buffers)
    {
        std::move(buffers.begin(), buffers.end(), std::back_inserter(m_inactiveBuffers));
    }

    unique_ptr<DynamicConstantBuffer> ManagerConstantBuffer::GetConstBuffer()
    {
        if (m_inactiveBuffers.empty())
            return CreateBuffer(m_bufferSize);
        unique_ptr<DynamicConstantBuffer> buffer = std::move(m_inactiveBuffers.back());
        m_inactiveBuffers.pop_back();
        buffer->Reset();
        return buffer;
    }

    unique_ptr<DynamicConstantBuffer> ManagerConstantBuffer::CreateBuffer(UINT64 size)
    {
        return make_unique<DynamicConstantBuffer>(m_device, size);
    }
}