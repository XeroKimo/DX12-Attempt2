#include "RendererDX12.h"
#include "Interfaces/DX12CommandAllocatorManager.h"

DX12CommandAllocatorManager::DX12CommandAllocatorManager()
{
}

void DX12CommandAllocatorManager::Initialize(DX12Device* device)
{
	m_device = device;
}

shared_ptr<DX12CommandAllocator> DX12CommandAllocatorManager::GetAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	shared_ptr<DX12CommandAllocator> allocator;
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		if (m_directAllocators.empty())
			allocator =CreateCommandAllocator(type);
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
		//break;
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
		break;
	}
	return allocator;
}

void DX12CommandAllocatorManager::ResetAllocators(std::vector<shared_ptr<DX12CommandAllocator>> allocators)
{
	switch (allocators[0]->GetType())
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		std::copy(allocators.begin(), allocators.end() - 1, m_directAllocators.begin());
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		std::copy(allocators.begin(), allocators.end() - 1, m_computeAllocators.begin());
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		std::copy(allocators.begin(), allocators.end() - 1, m_copyAllocators.begin());
		break;
	}
}


shared_ptr<DX12CommandAllocator> DX12CommandAllocatorManager::CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type)
{
	shared_ptr<DX12CommandAllocator> allocator = make_shared<DX12CommandAllocator>();
	allocator->Initialize(m_device, type);
	
	return allocator;
}
