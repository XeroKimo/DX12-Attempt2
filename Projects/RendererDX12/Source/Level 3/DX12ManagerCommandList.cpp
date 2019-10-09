#include "RendererDX12.h"
#include "Level 3/DX12ManagerCommandList.h"

namespace RendererDX12
{
    DX12ManagerCommandList::DX12ManagerCommandList(DX12BaseDevice* device, D3D12_COMMAND_LIST_TYPE type, std::vector<unique_ptr<DX12CommandQueue>>* commandQueues, DX12ManagerCommandAllocator* allocatorManager) :
        m_device(device),
        m_type(type),
        m_pCommandQueues(commandQueues),
        m_allocatorManager(allocatorManager),
        m_nodeMask(device->GetNodeMask())
    {
        //The following command list types does not require a manager
        assert(type != D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS);
        assert(type != D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE);
        assert(type != D3D12_COMMAND_LIST_TYPE_BUNDLE);
    }

    void DX12ManagerCommandList::CloseCommandList(unique_ptr<DX12CommandList>& list, UINT queueIndex)
    {
        assert(queueIndex < m_pCommandQueues->size());

        list->Close();
        m_waitingLists[queueIndex].AddCommandList(list);
    }

    void DX12ManagerCommandList::ExecuteCommandList(unique_ptr<DX12CommandList>& commandList, UINT queueIndex)
    {
        assert(queueIndex < m_pCommandQueues->size());

        commandList->Close();
        ID3D12CommandList* list[1] = { commandList->GetBase()->GetInterface() };
        (*m_pCommandQueues)[queueIndex]->GetInterface()->ExecuteCommandLists(1, list);
        (*m_pCommandQueues)[queueIndex]->SetActiveAllocator(commandList->GetCommandAllocator());
        m_inactiveList.push_back(std::move(commandList));
    }

    void DX12ManagerCommandList::ExecuteWaitingList(UINT queueIndex)
    {
        m_waitingLists[queueIndex].ExecuteWaitingList((*m_pCommandQueues)[queueIndex].get(), m_inactiveList);
    }

    void DX12ManagerCommandList::ExecuteAllWaitingLists()
    {
        for (size_t i = 0; i < m_waitingLists.size(); i++)
        {
            ExecuteWaitingList(static_cast<UINT>(i));
        }
    }

    unique_ptr<DX12CommandList> DX12ManagerCommandList::GetCommandList()
    {
        if (m_inactiveList.empty())
            return make_unique<DX12CommandList>(m_device->GetInterface(), m_nodeMask, m_type, std::move(m_allocatorManager->GetAllocator(m_type)));
        else
        {
            unique_ptr<DX12CommandList> list;
            m_inactiveList.back().swap(list);
            m_inactiveList.pop_back();
            list->Reset(std::move(m_allocatorManager->GetAllocator(m_type)));
            return list;
        }

    }

    void DX12ManagerCommandList::WaitingList::AddCommandList(unique_ptr<DX12CommandList>& commandList)
    {
        m_rawList.push_back(commandList->GetBase()->GetInterface());
        m_waitingAllocator.push_back(std::move(commandList->GetCommandAllocator()));
        m_waitingList.push_back(std::move(commandList));
    }

    void DX12ManagerCommandList::WaitingList::ExecuteWaitingList(DX12CommandQueue* commandQueue, std::vector<unique_ptr<DX12CommandList>>& inactiveList)
    {
        commandQueue->GetInterface()->ExecuteCommandLists(static_cast<UINT>(m_rawList.size()), m_rawList.data());
        commandQueue->SetActiveAllocators(m_waitingAllocator);
        std::move(m_waitingList.begin(), m_waitingList.end(), std::back_inserter(inactiveList));
        m_waitingList.clear();
        m_waitingAllocator.clear();
        m_rawList.clear();
    }
}