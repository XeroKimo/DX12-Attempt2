#pragma once
#include "DX12Header.h"
#include "Level 2/DX12CommandAllocator.h"
#include "Level 2/DX12ManagerConstBuffer.h"

class DX12ManagerCommandAllocator
{
public:
	DX12ManagerCommandAllocator();

	void Initialize(DX12BaseDevice* device, DX12ManagerConstBuffer* bufferManager);
	void ResetAllocators(std::vector<unique_ptr<DX12CommandAllocator>>& allocators);

	unique_ptr<DX12CommandAllocator> GetAllocator(const D3D12_COMMAND_LIST_TYPE& type);
private:
	unique_ptr<DX12CommandAllocator> CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
private:
    DX12BaseDevice* m_device;
	DX12ManagerConstBuffer* m_bufferManager;
	std::vector<unique_ptr<DX12CommandAllocator>> m_directAllocators;
	std::vector<unique_ptr<DX12CommandAllocator>> m_copyAllocators;
	std::vector<unique_ptr<DX12CommandAllocator>> m_computeAllocators;
};