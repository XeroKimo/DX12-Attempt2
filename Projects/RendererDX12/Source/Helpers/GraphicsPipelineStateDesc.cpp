#include "RendererDX12.h"
#include "Helpers/GraphicsPipelineStateDesc.h"

namespace RendererDX12
{
    namespace Helpers
    {
        GraphicsPipelineStateDesc::GraphicsPipelineStateDesc()
        {
            desc = D3D12_GRAPHICS_PIPELINE_STATE_DESC();
            D3D12_RASTERIZER_DESC rasterDesc;
            rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
            rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
            rasterDesc.FrontCounterClockwise = false;
            rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
            rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
            rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
            rasterDesc.DepthClipEnable = true;
            rasterDesc.MultisampleEnable = false;
            rasterDesc.AntialiasedLineEnable = false;
            rasterDesc.ForcedSampleCount = 0;
            rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

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
            blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

            D3D12_DEPTH_STENCIL_DESC depthDesc;
            depthDesc.DepthEnable = TRUE;
            depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
            depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
            depthDesc.StencilEnable = FALSE;
            depthDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
            depthDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
            const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp = { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
            depthDesc.FrontFace = defaultStencilOp;
            depthDesc.BackFace = defaultStencilOp;

            desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
            desc.RasterizerState = rasterDesc;
            desc.BlendState = blendDesc;
            desc.NumRenderTargets = 1;
            desc.DepthStencilState = depthDesc;
            desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        }

        void GraphicsPipelineStateDesc::GeneratePipelineState(ID3D12Device* device, D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version)
        {
            HRESULT hr = rootSignatureDesc.SerializeSignature(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1_0);
            assert(SUCCEEDED(hr));
            hr = device->CreateRootSignature(device->GetNodeCount(), rootSignatureDesc.GetSerializedSignature()->GetBufferPointer(), rootSignatureDesc.GetSerializedSignature()->GetBufferSize(), IID_PPV_ARGS(rootSiganture.GetAddressOf()));
            assert(SUCCEEDED(hr));

            desc.pRootSignature = rootSiganture.Get();
            desc.InputLayout = inputLayout.GetLayout();
            desc.VS = vsShader.GetByteCode();
            desc.PS = psShader.GetByteCode();
            desc.DS = dsShader.GetByteCode();
            desc.HS = hsShader.GetByteCode();
            desc.GS = gsShader.GetByteCode();

            hr = device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pipelineState.GetAddressOf()));
            assert(SUCCEEDED(hr));

        }
    }
}