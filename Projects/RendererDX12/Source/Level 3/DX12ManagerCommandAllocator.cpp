#include "RendererDX12.h"
#include "Level 3/DX12ManagerCommandAllocator.h"

namespace RendererDX12
{
    DX12ManagerCommandAllocator::DX12ManagerCommandAllocator(DX12BaseDevice* device, DX12ManagerConstBuffer* bufferManager) :
        m_device(device),
        m_bufferManager(bufferManager)
    {
    }

    unique_ptr<DX12CommandAllocator> DX12ManagerCommandAllocator::GetAllocator(const Command_List_Type& type)
    {
        unique_ptr<DX12CommandAllocator> allocator;
        switch (type)
        {
        case Command_List_Type::Direct:
            if (m_directAllocators.empty())
                return CreateCommandAllocator(type);
            else
            {
                allocator.swap(m_directAllocators.back());
                m_directAllocators.pop_back();
            }
            break;
        case  Command_List_Type::Compute:
            if (m_computeAllocators.empty())
                return CreateCommandAllocator(type);
            else
            {
                allocator.swap(m_computeAllocators.back());
                m_computeAllocators.pop_back();
            }
            break;
        case Command_List_Type::Copy:
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

    void DX12ManagerCommandAllocator::ResetAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocators)
    {
        switch (allocators[0]->GetBase()->GetType())
        {
        case Command_List_Type::Direct:
            std::move(allocators.begin(), allocators.end(), std::back_inserter(m_directAllocators));
            break;
        case Command_List_Type::Compute:
            std::move(allocators.begin(), allocators.end(), std::back_inserter(m_computeAllocators));
            break;
        case Command_List_Type::Copy:
            std::move(allocators.begin(), allocators.end(), std::back_inserter(m_copyAllocators));
            break;
        }
    }


    unique_ptr<DX12CommandAllocator> DX12ManagerCommandAllocator::CreateCommandAllocator(const Command_List_Type& type)
    {
        return make_unique<DX12CommandAllocator>(m_device->GetInterface(), type, m_bufferManager);
    }
}