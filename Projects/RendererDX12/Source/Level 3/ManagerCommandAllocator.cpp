#include "RendererDX12.h"

namespace RendererDX12
{
    ManagerCommandAllocator::ManagerCommandAllocator(BaseDevice* device, ManagerConstantBuffer* bufferManager) :
        m_device(device),
        m_bufferManager(bufferManager)
    {
    }

    unique_ptr<CommandAllocator> ManagerCommandAllocator::GetAllocator(const D3D12_COMMAND_LIST_TYPE& type)
    {
        unique_ptr<CommandAllocator> allocator;
        switch (type)
        {
        case D3D12_COMMAND_LIST_TYPE_DIRECT:
            if (m_directAllocators.empty())
                return CreateCommandAllocator(type);
            else
            {
                allocator.swap(m_directAllocators.back());
                m_directAllocators.pop_back();
            }
            break;
        case  D3D12_COMMAND_LIST_TYPE_COMPUTE:
            if (m_computeAllocators.empty())
                return CreateCommandAllocator(type);
            else
            {
                allocator.swap(m_computeAllocators.back());
                m_computeAllocators.pop_back();
            }
            break;
        case D3D12_COMMAND_LIST_TYPE_COPY:
            if (m_copyAllocators.empty())
                return CreateCommandAllocator(type);
            else
            {
                allocator.swap(m_copyAllocators.back());
                m_copyAllocators.pop_back();
            }
            break;
        default:
            assert(false);
            return nullptr;
            break;
        }
        allocator->Reset();
        return allocator;
    }

    void ManagerCommandAllocator::ResetAllocators(std::vector<unique_ptr<CommandAllocator>>& allocators)
    {
        switch (allocators[0]->GetBase()->GetType())
        {
        case D3D12_COMMAND_LIST_TYPE_DIRECT:
            std::move(allocators.begin(), allocators.end(), std::back_inserter(m_directAllocators));
            break;
        case D3D12_COMMAND_LIST_TYPE_COMPUTE:
            std::move(allocators.begin(), allocators.end(), std::back_inserter(m_computeAllocators));
            break;
        case D3D12_COMMAND_LIST_TYPE_COPY:
            std::move(allocators.begin(), allocators.end(), std::back_inserter(m_copyAllocators));
            break;
        }
    }


    unique_ptr<CommandAllocator> ManagerCommandAllocator::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
    {
        return make_unique<CommandAllocator>(m_device->GetInterface(), type, m_bufferManager);
    }
}