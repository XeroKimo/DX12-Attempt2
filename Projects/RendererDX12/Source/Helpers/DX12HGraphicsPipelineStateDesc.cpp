#include "RendererDX12.h"
#include "Helpers/DX12HGraphicsPipelineStateDesc.h"

DX12HGraphicsPipelineStateDesc::DX12HGraphicsPipelineStateDesc()
{
    desc = D3D12_GRAPHICS_PIPELINE_STATE_DESC();
	D3D12_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterDesc.ForcedSampleCount = 0;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0;

	D3D12_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].LogicOpEnable = false;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D12_COLOR_WRITE_ENABLE_ALL;

    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    desc.RasterizerState = rasterDesc;
    desc.BlendState = blendDesc;
    desc.NumRenderTargets = 1;
}

void DX12HGraphicsPipelineStateDesc::SetShaders(D3D12_SHADER_BYTECODE* VS, D3D12_SHADER_BYTECODE* PS, D3D12_SHADER_BYTECODE* DS, D3D12_SHADER_BYTECODE* HS, D3D12_SHADER_BYTECODE* GS)
{
    if (VS)
        desc.VS = *VS;
    if (PS)
        desc.PS = *PS;
    if (DS)
        desc.DS = *DS;
    if (HS)
        desc.HS = *HS;
    if (GS)
        desc.GS = *GS;
}