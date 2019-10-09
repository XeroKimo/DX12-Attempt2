#pragma once
#include "DX12Header.h"
#include "RootSignatureDesc.h"
#include "ShaderByteCode.h"
#include "ShaderInputLayout.h"

namespace RendererDX12
{
    namespace Helpers
    {
        struct GraphicsPipelineStateDesc
        {
        public:
            GraphicsPipelineStateDesc();
            void GeneratePipelineState(ID3D12Device* device, D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version);
        public:
            ComPtr<ID3D12RootSignature> rootSiganture;
            ComPtr<ID3D12PipelineState> pipelineState;
        public:
            D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
            RootSignatureDesc rootSignatureDesc = {};
            ShaderInputLayout inputLayout = {};
            ShaderByteCode vsShader = {};
            ShaderByteCode psShader = {};
            ShaderByteCode dsShader = {};
            ShaderByteCode hsShader = {};
            ShaderByteCode gsShader = {};
        };
    }
}