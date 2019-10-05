#pragma once
#include "DX12Header.h"

class DX12PipelineState
{
public:
	DX12PipelineState() {}
	inline void Initialize(ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature) { m_pipelineState = pipelineState; m_rootSignature = rootSignature; }
	inline void SetGraphicsRootSignature(ID3D12GraphicsCommandList* commandList) { commandList->SetGraphicsRootSignature(m_rootSignature.Get()); }
	inline void SetComputeRootSignature(ID3D12GraphicsCommandList* commandList) { commandList->SetComputeRootSignature(m_rootSignature.Get()); }
	inline void SetPipelineState(ID3D12GraphicsCommandList* commandList) { commandList->SetPipelineState(m_pipelineState.Get()); }

private:
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12RootSignature> m_rootSignature;
};