#include "RendererDX12.h"

namespace RendererDX12
{
    CommandList::CommandList(ID3D12Device* device, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, unique_ptr<CommandAllocator> allocator, ManagerConstantBuffer* constantBufferManager) :
        m_commandList(device, nodeMask, type, allocator->GetBase()->GetInterface()),
        m_allocator(),
        m_nodeMask(nodeMask),
        m_constantBufferManager(constantBufferManager)
    {
        m_allocator.swap(allocator);
    }

    void CommandList::Reset(unique_ptr<CommandAllocator> allocator)
    {
        m_commandList.GetInterface()->Reset(allocator->GetBase()->GetInterface(), nullptr);
        m_allocator.swap(allocator);
        m_constantBuffers.clear();
    }

    void CommandList::SetConstantBuffer(UINT rootParamIndex, void* data, UINT64 size)
    {
        if (!m_constantBufferManager)
            return;
        unique_ptr<UploadBuffer> buffer;
        if (m_constantBuffers.empty())
        {
            buffer = m_constantBufferManager->GetConstBuffer();
            m_constantBuffers.push_back(buffer.get());
            m_allocator->AttachConstantBuffer(std::move(buffer));
        }
        if (!m_constantBuffers.back()->HasSpace(size))
        {
            buffer = m_constantBufferManager->GetConstBuffer();
            m_constantBuffers.push_back(buffer.get());
            m_allocator->AttachConstantBuffer(std::move(buffer));
        }

        m_commandList.GetInterface()->SetGraphicsRootConstantBufferView(rootParamIndex, m_constantBuffers.back()->UploadCBVSRVUAV(data, size));
    }

    void CommandList::UploadData(ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data)
    {
        m_allocator->UploadData(m_commandList.GetInterface(), m_nodeMask, destination, data);
    }
}