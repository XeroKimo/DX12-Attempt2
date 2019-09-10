#include "RendererDX12.h"
#include "Interfaces/DX12CommandAllocator.h"

DX12CommandAllocator::DX12CommandAllocator()
{
}

void DX12CommandAllocator::Initialize(DX12Device* device, DX12CommandAllocatorManager* manager, const D3D12_COMMAND_LIST_TYPE& type)
{
	m_manager = manager;
	m_type = type;

	if (FAILED(device->GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(m_allocator.GetAddressOf()))))
		assert(false);
}

void DX12CommandAllocator::SetActive()
{
	m_manager->SetAllocatorActive(shared_from_this());
}
