#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    namespace Helpers
    {
        class RootDescriptorTable
        {
        public:
            void AddTable(UINT numDescriptors, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT baseShaderRegister = 0, UINT registerSpace = 0, UINT offsetDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);

            size_t GetRangesSize() { return m_descriptorRanges.size(); }
            D3D12_DESCRIPTOR_RANGE* GetRanges() { return m_descriptorRanges.data(); }
        private:
            std::vector<D3D12_DESCRIPTOR_RANGE> m_descriptorRanges;
        };

        struct RootSignatureDesc
        {

        public:
            void CreateRootConstant(UINT numValues, UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_ALL);
            void CreateRootDescriptor(D3D12_ROOT_PARAMETER_TYPE paramType, UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_ALL);
            void CreateRootDescriptorTable(RootDescriptorTable& table, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_ALL);
            void AddStaticSampler(D3D12_STATIC_SAMPLER_DESC sampler) { m_staticSamplers.push_back(sampler); }

            HRESULT SerializeSignature(D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version);
            ID3DBlob* GetSerializedSignature() { return m_signatureBlob.Get(); }
            ID3DBlob* GetError() { return m_errorBlob.Get(); }
        private:
            std::vector<D3D12_ROOT_PARAMETER> m_parameters;
            std::vector<D3D12_STATIC_SAMPLER_DESC> m_staticSamplers;

            ComPtr<ID3DBlob> m_signatureBlob;
            ComPtr<ID3DBlob> m_errorBlob;
            UINT DWORDRemaining = 64;
        };
    }
}