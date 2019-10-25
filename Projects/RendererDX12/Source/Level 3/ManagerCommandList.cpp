#include "RendererDX12.h"

namespace RendererDX12
{
    ManagerCommandList::ManagerCommandList(BaseDevice* device, D3D12_COMMAND_LIST_TYPE type, std::vector<unique_ptr<CommandQueue>>* commandQueues, ManagerCommandAllocator* allocatorManager, ManagerConstantBuffer* constantBufferManager) :
        m_device(device),
        m_type(type),
        m_pCommandQueues(commandQueues),
        m_allocatorManager(allocatorManager),
        m_nodeMask(device->GetNodeMask()),
        m_constantBufferManager(constantBufferManager)
    {
        //The following command list types does not require a manager
        assert(type != D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS);
        assert(type != D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE);
        assert(type != D3D12_COMMAND_LIST_TYPE_BUNDLE);
    }

    void ManagerCommandList::CloseCommandList(unique_ptr<CommandList>& list, UINT queueIndex)
    {
        assert(queueIndex < m_pCommandQueues->size());

        list->Close();
        m_waitingLists[queueIndex].AddCommandList(list);
    }

    void ManagerCommandList::ExecuteCommandList(unique_ptr<CommandList>& commandList, UINT queueIndex)
    {
        assert(queueIndex < m_pCommandQueues->size());

        commandList->Close();
        ID3D12CommandList* list[1] = { commandList->GetBase()->GetInterface() };
        (*m_pCommandQueues)[queueIndex]->GetInterface()->ExecuteCommandLists(1, list);
        (*m_pCommandQueues)[queueIndex]->SetActiveAllocator(commandList->GetCommandAllocator());
        m_inactiveList.push_back(std::move(commandList));
    }

    void ManagerCommandList::ExecuteWaitingList(UINT queueIndex)
    {
        m_waitingLists[queueIndex].ExecuteWaitingList((*m_pCommandQueues)[queueIndex].get(), m_inactiveList);
    }

    void ManagerCommandList::ExecuteAllWaitingLists()
    {
        for (size_t i = 0; i < m_waitingLists.size(); i++)
        {
            ExecuteWaitingList(static_cast<UINT>(i));
        }
    }

    unique_ptr<CommandList> ManagerCommandList::GetCommandList()
    {
        if (m_inactiveList.empty())
            return make_unique<CommandList>(m_device, std::move(m_allocatorManager->GetAllocator(m_type)), m_constantBufferManager);
        else
        {
            unique_ptr<CommandList> list;
            m_inactiveList.back().swap(list);
            m_inactiveList.pop_back();
            list->Reset(std::move(m_allocatorManager->GetAllocator(m_type)));
            return list;
        }

    }

    void ManagerCommandList::WaitingList::AddCommandList(unique_ptr<CommandList>& commandList)
    {
        m_rawList.push_back(commandList->GetBase()->GetInterface());
        m_waitingAllocator.push_back(std::move(commandList->GetCommandAllocator()));
        m_waitingList.push_back(std::move(commandList));
    }

    void ManagerCommandList::WaitingList::ExecuteWaitingList(CommandQueue* commandQueue, std::vector<unique_ptr<CommandList>>& inactiveList)
    {
        commandQueue->GetInterface()->ExecuteCommandLists(static_cast<UINT>(m_rawList.size()), m_rawList.data());
        commandQueue->SetActiveAllocators(m_waitingAllocator);
        std::move(m_waitingList.begin(), m_waitingList.end(), std::back_inserter(inactiveList));
        m_waitingList.clear();
        m_waitingAllocator.clear();
        m_rawList.clear();
    }
}