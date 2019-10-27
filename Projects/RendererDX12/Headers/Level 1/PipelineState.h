#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class PipelineState
    {
    public:
        PipelineState(ComPtr<ID3D12PipelineState> const pipelineState, ComPtr<ID3D12RootSignature> const rootSignature) { m_pipelineState = pipelineState; m_rootSignature = rootSignature; }
        inline void SetGraphicsRootSignature(const BaseCommandList* const commandList) { commandList->GetInterface()->SetGraphicsRootSignature(m_rootSignature.Get()); }
        inline void SetComputeRootSignature(const BaseCommandList* const commandList) { commandList->GetInterface()->SetComputeRootSignature(m_rootSignature.Get()); }
        inline void SetPipelineState(const BaseCommandList* const commandList) { commandList->GetInterface()->SetPipelineState(m_pipelineState.Get()); }

    private:
        ComPtr<ID3D12PipelineState> m_pipelineState;
        ComPtr<ID3D12RootSignature> m_rootSignature;
    };
}