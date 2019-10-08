#include "RendererDX12.h"
#include "Level 3/DX12ManagerCommandAllocator.h"

DX12ManagerCommandAllocator::DX12ManagerCommandAllocator() :
	m_device(nullptr),
    m_bufferManager(nullptr)
{
}

void DX12ManagerCommandAllocator::Initialize(DX12BaseDevice* device, DX12ManagerConstBuffer* bufferManager)
{
	m_device = device;
	m_bufferManager = bufferManager;
}

unique_ptr<DX12CommandAllocator> DX12ManagerCommandAllocator::GetAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	unique_ptr<DX12CommandAllocator> allocator;
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
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
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

void DX12ManagerCommandAllocator::ResetAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocators)
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


unique_ptr<DX12CommandAllocator> DX12ManagerCommandAllocator::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	unique_ptr<DX12CommandAllocator> allocator = make_unique<DX12CommandAllocator>();
	allocator->Initialize(m_device->GetInterface(), type, m_bufferManager);
	
	return allocator;
}
