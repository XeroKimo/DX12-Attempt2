#pragma once
#include "DX12Header.h"
#include "Level 1/DX12BaseCommandAllocator.h"
#include "Level 1/DX12UploadBuffer.h"

namespace RendererDX12
{
class DX12ManagerConstBuffer;

    class DX12CommandAllocator
    {
    public:
        DX12CommandAllocator(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type, DX12ManagerConstBuffer* bufferManager);

        D3D12_GPU_VIRTUAL_ADDRESS UploadDynamicCBV(void* data, UINT64 size);
        void UploadData(ID3D12GraphicsCommandList* commandList, UINT nodeMask, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data);
        void UploadData(shared_ptr<DX12UploadBuffer> buffer) { m_temporaryBuffers.push_back(buffer); }
        void Reset();

        inline DX12BaseCommandAllocator* GetBase() { return &m_commandAllocator; }
        inline ID3D12CommandAllocator* GetInterface() { return m_commandAllocator.GetInterface(); }
    private:
        DX12ManagerConstBuffer* m_bufferManager;
        DX12BaseCommandAllocator m_commandAllocator;
        std::vector<unique_ptr<DX12UploadBuffer>> m_constBuffers;
        std::vector<shared_ptr<DX12UploadBuffer>> m_temporaryBuffers;
    };
}