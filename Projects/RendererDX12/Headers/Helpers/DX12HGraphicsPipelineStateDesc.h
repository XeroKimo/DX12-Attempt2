#pragma once
#include "DX12Header.h"

struct DX12HGraphicsPipelineStateDesc
{
public:
	DX12HGraphicsPipelineStateDesc();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;

    void SetShaders(D3D12_SHADER_BYTECODE* VS, D3D12_SHADER_BYTECODE* PS, D3D12_SHADER_BYTECODE* DS = nullptr, D3D12_SHADER_BYTECODE* HS = nullptr, D3D12_SHADER_BYTECODE* GS = nullptr );
};