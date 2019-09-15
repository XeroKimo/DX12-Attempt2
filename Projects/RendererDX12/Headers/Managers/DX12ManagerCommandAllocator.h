#pragma once
#include "DX12Header.h"
class DX12BaseDevice;
class DX12ManagerUploadBuffer;

class DX12ManagerCommandAllocator
{
public:
	DX12ManagerCommandAllocator();

	void Initialize(ID3D12Device* device, DX12ManagerUploadBuffer* bufferManager);

	shared_ptr<DX12CommandAllocator>  GetAllocator(const D3D12_COMMAND_LIST_TYPE& type);
	void ResetAllocators(std::vector<shared_ptr<DX12CommandAllocator>> allocators);
private:
	shared_ptr<DX12CommandAllocator> CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
private:
    ID3D12Device* m_device;
	DX12ManagerUploadBuffer* m_bufferManager;
	std::vector<shared_ptr<DX12CommandAllocator>> m_directAllocators;
	//std::vector<shared_ptr<DX12CommandAllocator>> m_bundleAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_copyAllocators;
	std::vector<shared_ptr<DX12CommandAllocator>> m_computeAllocators;
};