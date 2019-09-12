#pragma once
#include "DX12Header.h"

struct DX12HGraphicsPipelineStateDesc
{
public:
	DX12HGraphicsPipelineStateDesc();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
};