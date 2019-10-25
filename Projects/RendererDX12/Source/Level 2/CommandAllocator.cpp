#include "RendererDX12.h"

namespace RendererDX12
{
    CommandAllocator::CommandAllocator(BaseDevice* device, const D3D12_COMMAND_LIST_TYPE& type) :
        m_commandAllocator(device, type)
    {
    }

    void CommandAllocator::UploadData(BaseCommandList* commandList, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data)
    {
        m_temporaryBuffers.push_back(make_shared<UploadBuffer>(commandList, destination, data));
    }

    void CommandAllocator::Reset()
    {
        m_commandAllocator.GetInterface()->Reset();
        m_constBuffers.clear();
        m_temporaryBuffers.clear();
    }
}