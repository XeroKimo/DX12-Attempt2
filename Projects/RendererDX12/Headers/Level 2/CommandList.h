#pragma once
#include "DX12Header.h"
#include "Level 1/BaseCommandList.h"

namespace RendererDX12
{
    class ManagerConstantBuffer;
    class CommandList
    {
    public:
        CommandList(BaseDevice* device, unique_ptr<CommandAllocator> allocator, ManagerConstantBuffer* constantBufferManager);
        void Reset(unique_ptr<CommandAllocator> allocator);

        void SetConstantBuffer(UINT rootParamIndex, void* data, UINT64 size);
        void UploadData(ID3D12Resource* destination, D3D12_SUBRESOURCE_DATA* data);
        void UploadData(shared_ptr<UploadBuffer> buffer) { m_allocator->UploadData(buffer); }

        inline void Close() { m_commandList.GetInterface()->Close(); }
        inline ID3D12GraphicsCommandList* GetInterface() { return m_commandList.GetInterface(); }
        inline BaseCommandList* GetBase() { return &m_commandList; }
        inline unique_ptr<CommandAllocator>& GetCommandAllocator() { return m_allocator; }

    private:
        void AttachNewConstantBuffer();
    private:
        unique_ptr<CommandAllocator> m_allocator;
        BaseCommandList m_commandList;

        ManagerConstantBuffer* m_constantBufferManager;
        DynamicConstantBuffer* m_currentConstantBuffer;
    };
}