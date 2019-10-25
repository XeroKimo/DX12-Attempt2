#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    class PipelineState
    {
    public:
        PipelineState(ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature) { m_pipelineState = pipelineState; m_rootSignature = rootSignature; }
        inline void SetGraphicsRootSignature(BaseCommandList* commandList) { commandList->GetInterface()->SetGraphicsRootSignature(m_rootSignature.Get()); }
        inline void SetComputeRootSignature(BaseCommandList* commandList) { commandList->GetInterface()->SetComputeRootSignature(m_rootSignature.Get()); }
        inline void SetPipelineState(BaseCommandList* commandList) { commandList->GetInterface()->SetPipelineState(m_pipelineState.Get()); }

    private:
        ComPtr<ID3D12PipelineState> m_pipelineState;
        ComPtr<ID3D12RootSignature> m_rootSignature;
    };
}