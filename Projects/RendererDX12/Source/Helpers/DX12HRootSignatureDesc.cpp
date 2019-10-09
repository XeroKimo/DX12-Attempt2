#include "RendererDX12.h"
#include "Helpers/DX12HRootSignatureDesc.h"

namespace RendererDX12
{
    void DX12HRootSignatureDesc::CreateRootConstant(UINT numValues, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisiblity)
    {
        assert(DWORDRemaining - numValues >= 0);

        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        param.Constants.Num32BitValues = numValues;
        param.Constants.ShaderRegister = shaderRegister;
        param.Constants.RegisterSpace = registerSpace;
        param.ShaderVisibility = shaderVisiblity;

        m_parameters.push_back(param);
        DWORDRemaining -= numValues;
    }

    void DX12HRootSignatureDesc::CreateRootDescriptor(D3D12_ROOT_PARAMETER_TYPE paramType, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisiblity)
    {
        assert(paramType != D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS && paramType != D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);
        assert(DWORDRemaining - 2 >= 0);

        D3D12_ROOT_PARAMETER param;
        param.ParameterType = paramType;
        param.Descriptor.ShaderRegister = shaderRegister;
        param.Descriptor.RegisterSpace = registerSpace;
        param.ShaderVisibility = shaderVisiblity;

        m_parameters.push_back(param);
        DWORDRemaining -= 2;
    }

    void DX12HRootSignatureDesc::CreateRootDescriptorTable(DX12HRootSignatureDesc::DescriptorTable& table, D3D12_SHADER_VISIBILITY shaderVisiblity)
    {
        assert(DWORDRemaining - 1 >= 0);

        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(table.m_descriptorRanges.size());
        param.DescriptorTable.pDescriptorRanges = table.m_descriptorRanges.data();
        param.ShaderVisibility = shaderVisiblity;


        m_parameters.push_back(param);
        DWORDRemaining--;

    }

    HRESULT DX12HRootSignatureDesc::SerializeSignature(D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version)
    {
        D3D12_ROOT_SIGNATURE_DESC desc;
        desc.NumParameters = static_cast<UINT>(m_parameters.size());
        desc.pParameters = m_parameters.data();
        desc.NumStaticSamplers = static_cast<UINT>(m_staticSamplers.size());
        desc.pStaticSamplers = m_staticSamplers.data();
        desc.Flags = flags;

        return D3D12SerializeRootSignature(&desc, version, &m_signatureBlob, &m_errorBlob);
    }

    void DX12HRootSignatureDesc::DescriptorTable::AddTable(UINT numDescriptors, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT baseShaderRegister, UINT registerSpace, UINT offsetDescriptorsFromTableStart)
    {
        D3D12_DESCRIPTOR_RANGE range;
        range.NumDescriptors = numDescriptors;
        range.RangeType = rangeType;
        range.BaseShaderRegister = baseShaderRegister;
        range.RegisterSpace = registerSpace;
        range.OffsetInDescriptorsFromTableStart = offsetDescriptorsFromTableStart;

        m_descriptorRanges.push_back(range);
    }
}