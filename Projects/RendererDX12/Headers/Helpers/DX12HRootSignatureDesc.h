#pragma once
#include "DX12Header.h"

class DX12HRootSignatureDesc
{
public:
    class DescriptorTable
    {
    public:

        friend DX12HRootSignatureDesc;
        void AddTable(UINT numDescriptors, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT baseShaderRegister = 0, UINT registerSpace = 0, UINT offsetDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);
    private:
        std::vector<D3D12_DESCRIPTOR_RANGE> m_descriptorRanges;
    };

public:
	void CreateRootConstant(UINT numValues, UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_ALL);
    void CreateRootDescriptor(D3D12_ROOT_PARAMETER_TYPE paramType, UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_ALL);
    void CreateRootDescriptorTable(DX12HRootSignatureDesc::DescriptorTable table, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_ALL);
    void AddStaticsSampler(D3D12_STATIC_SAMPLER_DESC sampler) { m_staticSamplers.push_back(sampler); }

    HRESULT GetSerializedRootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version, ID3DBlob** ppBlob, ID3DBlob** ppErrorBlob = nullptr);
private:
	std::vector<D3D12_ROOT_PARAMETER> m_parameters;
    std::vector<D3D12_STATIC_SAMPLER_DESC> m_staticSamplers;
	UINT DWORDRemaining = 64;
};