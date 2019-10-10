#pragma once
#include "DX12Header.h"
#include "Level 2/CommandAllocator.h"
#include "Level 2/ManagerConstantBuffer.h"

namespace RendererDX12
{
    class ManagerCommandAllocator
    {
    public:
        ManagerCommandAllocator( BaseDevice* device, ManagerConstantBuffer* bufferManager);

        void ResetAllocators(std::vector<unique_ptr<CommandAllocator>>& allocators);

        unique_ptr<CommandAllocator> GetAllocator(const D3D12_COMMAND_LIST_TYPE& type);
    private:
        unique_ptr<CommandAllocator> CreateCommandAllocator(const D3D12_COMMAND_LIST_TYPE& type);
    private:
         BaseDevice* m_device;
        ManagerConstantBuffer* m_bufferManager;
        std::vector<unique_ptr<CommandAllocator>> m_directAllocators;
        std::vector<unique_ptr<CommandAllocator>> m_copyAllocators;
        std::vector<unique_ptr<CommandAllocator>> m_computeAllocators;
    };
}