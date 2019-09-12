#include "RendererDX12.h"
#include "Interfaces/DX12CommandAllocator.h"

DX12CommandAllocator::DX12CommandAllocator() :
	m_type(D3D12_COMMAND_LIST_TYPE_DIRECT)
{
}

void DX12CommandAllocator::Initialize(DX12Device* device, const D3D12_COMMAND_LIST_TYPE& type)
{
	m_type = type;
	HRESULT hr = device->GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(m_allocator.GetAddressOf()));
	assert(SUCCEEDED(hr));
}
