#include "RendererDX12.h"

namespace RendererDX12
{
    CommandAllocator::CommandAllocator(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type, ManagerConstantBuffer* bufferManager) :
        m_commandAllocator(device, type)
    {
        m_bufferManager = bufferManager;
    }

    D3D12_GPU_VIRTUAL_ADDRESS CommandAllocator::UploadDynamicCBV(void* data, UINT64 size)
    {
        if (m_constBuffers.empty())
            m_constBuffers.push_back(m_bufferManager->GetConstBuffer());

        if (!m_constBuffers.back()->HasSpace(size))
            m_constBuffers.push_back(m_bufferManager->GetConstBuffer());

        return m_constBuffers.back()->UploadCBVSRVUAV(data, size);
    }

    void CommandAllocator::UploadData(ID3D12GraphicsCommandList* commandList, UINT nodeMask, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data)
    {
        m_temporaryBuffers.push_back(make_shared<UploadBuffer>(commandList, nodeMask, destination, data));
    }

    void CommandAllocator::Reset()
    {
        m_commandAllocator.GetInterface()->Reset();
        m_bufferManager->ResetBuffers(m_constBuffers);
        m_constBuffers.clear();
        m_temporaryBuffers.clear();
    }
}