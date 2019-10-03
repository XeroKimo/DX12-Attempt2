#include "RendererDX12.h"
#include "Level 1/DX12BaseCommandAllocator.h"

DX12BaseCommandAllocator::DX12BaseCommandAllocator() :
	m_type(D3D12_COMMAND_LIST_TYPE_DIRECT)
{
}

void DX12BaseCommandAllocator::Initialize(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type)
{
	m_type = type;
	HRESULT hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(m_allocator.GetAddressOf()));
    //hr = device->GetDeviceRemovedReason();
	assert(SUCCEEDED(hr));
}