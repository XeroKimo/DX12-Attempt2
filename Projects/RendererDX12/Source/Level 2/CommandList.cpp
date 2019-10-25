#include "RendererDX12.h"

namespace RendererDX12
{
    CommandList::CommandList(BaseDevice* device, unique_ptr<CommandAllocator> allocator, ManagerConstantBuffer* constantBufferManager) :
        m_commandList(device, allocator->GetBase()),
        m_allocator(),
        m_constantBufferManager(constantBufferManager),
        m_currentConstantBuffer(nullptr)
    {
        m_allocator.swap(allocator);
    }

    void CommandList::Reset(unique_ptr<CommandAllocator> allocator)
    {
        m_commandList.GetInterface()->Reset(allocator->GetBase()->GetInterface(), nullptr);
        m_allocator.swap(allocator);
        m_currentConstantBuffer = nullptr;
    }

    void CommandList::SetConstantBuffer(UINT rootParamIndex, void* data, UINT64 size)
    {
        if (!m_constantBufferManager)
            return;

        if (!m_currentConstantBuffer)
        {
            AttachNewConstantBuffer();
        }
        if (!m_currentConstantBuffer->HasSpace(size))
        {
            AttachNewConstantBuffer();
        }

        m_commandList.GetInterface()->SetGraphicsRootConstantBufferView(rootParamIndex, m_currentConstantBuffer->Upload(data, size));
    }

    void CommandList::UploadData(ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data)
    {
        m_allocator->UploadData(&m_commandList, destination, data);
    }

    void CommandList::AttachNewConstantBuffer()
    {
        unique_ptr<DynamicConstantBuffer> buffer;
        buffer = m_constantBufferManager->GetConstBuffer();
        m_currentConstantBuffer = buffer.get();
        m_allocator->AttachConstantBuffer(std::move(buffer));
    }
}