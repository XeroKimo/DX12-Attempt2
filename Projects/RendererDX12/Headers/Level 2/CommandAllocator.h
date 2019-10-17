#pragma once
#include "DX12Header.h"
#include "Level 1/BaseCommandAllocator.h"
#include "Level 1/UploadBuffer.h"

namespace RendererDX12
{
    class ManagerConstantBuffer;

    class CommandAllocator
    {
    public:
        CommandAllocator(ID3D12Device* device, const D3D12_COMMAND_LIST_TYPE& type);

        void UploadData(ID3D12GraphicsCommandList* commandList, UINT nodeMask, ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data);
        void UploadData(shared_ptr<UploadBuffer> buffer) { m_temporaryBuffers.push_back(buffer); }
        void Reset();

        void AttachConstantBuffer(unique_ptr<UploadBuffer> buffer) { m_constBuffers.push_back(std::move(buffer)); }

        inline BaseCommandAllocator* GetBase() { return &m_commandAllocator; }
        inline ID3D12CommandAllocator* GetInterface() { return m_commandAllocator.GetInterface(); }
        std::vector<unique_ptr<UploadBuffer>>& GetConstantBuffers() { return m_constBuffers; }
    private:
        BaseCommandAllocator m_commandAllocator;
        std::vector<unique_ptr<UploadBuffer>> m_constBuffers;
        std::vector<shared_ptr<UploadBuffer>> m_temporaryBuffers;
    };
}