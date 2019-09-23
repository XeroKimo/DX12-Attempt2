#include "RendererDX12.h"
#include "Level 2/DX12ManagerCommandAllocator.h"

DX12ManagerCommandAllocator::DX12ManagerCommandAllocator() :
	m_device(nullptr),
    m_bufferManager(nullptr)
{
}

void DX12ManagerCommandAllocator::Initialize(ID3D12Device* device, DX12ManagerUploadBuffer* bufferManager)
{
	m_device = device;
	m_bufferManager = bufferManager;
}

shared_ptr<DX12CommandAllocator> DX12ManagerCommandAllocator::GetAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	shared_ptr<DX12CommandAllocator> allocator;
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		if (m_directAllocators.empty())
			allocator = CreateCommandAllocator(type);
		else
		{
			allocator = m_directAllocators.back();
			m_directAllocators.pop_back();
		}
		break;
	case D3D12_COMMAND_LIST_TYPE_BUNDLE:
		//if (m_bundleAllocators.empty())
		//	allocator = CreateCommandAllocator(type);
		//else
		//{
		//	allocator = m_bundleAllocators.back();
		//	m_bundleAllocators.pop_back();
		//}
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		if (m_computeAllocators.empty())
			allocator = CreateCommandAllocator(type);
		else
		{
			allocator = m_computeAllocators.back();
			m_computeAllocators.pop_back();
		}
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		if (m_copyAllocators.empty())
			allocator = CreateCommandAllocator(type);
		else
		{
			allocator = m_copyAllocators.back();
			m_copyAllocators.pop_back();
		}
		break;
	default:
        assert(false);
		break;
	}

	return allocator;
}

void DX12ManagerCommandAllocator::ResetAllocators(std::vector<shared_ptr<DX12CommandAllocator>> allocators)
{
	switch (allocators[0]->GetBase()->GetType())
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		std::copy(allocators.begin(), allocators.end(), std::back_inserter(m_directAllocators));
		for (shared_ptr<DX12CommandAllocator>& allocator : m_directAllocators)
			allocator->Reset();
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		std::copy(allocators.begin(), allocators.end(), std::back_inserter(m_computeAllocators));
		for (shared_ptr<DX12CommandAllocator>& allocator : m_computeAllocators)
			allocator->Reset();
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		std::copy(allocators.begin(), allocators.end(), std::back_inserter(m_copyAllocators));
		for (shared_ptr<DX12CommandAllocator>& allocator : m_copyAllocators)
			allocator->Reset();
		break;
	}
}


shared_ptr<DX12CommandAllocator> DX12ManagerCommandAllocator::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	shared_ptr<DX12CommandAllocator> allocator = make_shared<DX12CommandAllocator>();
	allocator->Initialize(m_device, type, this);
	
	return allocator;
}
