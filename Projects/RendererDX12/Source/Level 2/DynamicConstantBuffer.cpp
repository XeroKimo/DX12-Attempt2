#include "RendererDX12.h"

namespace RendererDX12
{
    DynamicConstantBuffer::DynamicConstantBuffer(BaseDevice* device, UINT64 size) :
        m_uploadBuffer(device, size)
    {
        D3D12_RANGE range = { 0,0 };
        m_uploadBuffer.GetInterface()->Map(0, &range, reinterpret_cast<void**>(&m_mappedAddress));
    }

    D3D12_GPU_VIRTUAL_ADDRESS DynamicConstantBuffer::Upload(void* data, UINT64 size)
    {
        D3D12_GPU_VIRTUAL_ADDRESS address = m_uploadBuffer.GetInterface()->GetGPUVirtualAddress() + m_usedSize;
        memcpy(m_mappedAddress + m_usedSize, data, static_cast<size_t>(size));
        m_usedSize += (static_cast<size_t>(size) + ConstBufferAlignment - 1) & ~(ConstBufferAlignment - 1);
        return address;
    }

    void DynamicConstantBuffer::Reset()
    {
        ZeroMemory(m_mappedAddress, static_cast<size_t>(m_totalSize));
        m_usedSize = 0;
    }
}