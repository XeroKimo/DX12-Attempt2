#pragma once
#include "DX12Header.h"
#include "Interfaces/DX12CommandAllocator.h"
class DX12Device;

class DX12MCommandAllocatorManager
{
public:
	DX12MCommandAllocatorManager();

	void Initialize(ID3D12Device* device);

	shared_ptr<DX12CommandAllocator>  GetAllocator(const D3D12_COMMAND_LIST_TYPE& type);
	void ResetAllocators(std::vector<shared_ptr<DX12CommandAllocator>> allocators);
private:
	shared_ptr<DX12CommandAllocator> CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
private:
    ID3D12Device* m_device;

	std::vector<shared_ptr<DX12CommandAllocator>> m_directAllocators;
	//std::vector<shared_ptr<DX12CommandAllocator>> m_bundleAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_copyAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_computeAllocators;
};