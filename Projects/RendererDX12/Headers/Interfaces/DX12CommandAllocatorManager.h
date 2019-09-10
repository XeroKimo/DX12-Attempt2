#pragma once
#include "DX12Header.h"
#include "DX12CommandAllocator.h"
class DX12Device;

class DX12CommandAllocatorManager
{
public:
	DX12CommandAllocatorManager();

	void Initialize(DX12Device* device);

	shared_ptr<DX12CommandAllocator>  GetAllocator(const D3D12_COMMAND_LIST_TYPE& type);
	void SetAllocatorActive(shared_ptr<DX12CommandAllocator>  commandAllocator);

	void ResetAllocators();
private:
	shared_ptr<DX12CommandAllocator> CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
private:
	DX12Device* m_device;

	std::vector<shared_ptr<DX12CommandAllocator>> m_directAllocators;
	//std::vector<shared_ptr<DX12CommandAllocator>> m_bundleAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_copyAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_computeAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_activeAllocators;
};