#pragma once
#include "DX12Header.h"
#include "DX12HShaderByteCode.h"
#include "DX12HRootSignatureDesc.h"
#include "DX12HInputLayout.h"

struct DX12HGraphicsPipelineStateDesc
{
public:
	DX12HGraphicsPipelineStateDesc();
	void GeneratePipelineState(ID3D12Device* device, D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version);
public:
	ComPtr<ID3D12RootSignature> rootSiganture;
	ComPtr<ID3D12PipelineState> pipelineState;
public:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	DX12HRootSignatureDesc rootSignatureDesc;
	DX12HInputLayout inputLayout;
	DX12HShaderByteCode vsShader;
	DX12HShaderByteCode psShader;
	DX12HShaderByteCode dsShader;
	DX12HShaderByteCode hsShader;
	DX12HShaderByteCode gsShader;

    //void SetShaders(D3D12_SHADER_BYTECODE* VS, D3D12_SHADER_BYTECODE* PS, D3D12_SHADER_BYTECODE* DS = nullptr, D3D12_SHADER_BYTECODE* HS = nullptr, D3D12_SHADER_BYTECODE* GS = nullptr );
};