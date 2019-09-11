#include "RendererDX12.h"

void DX12HRootParameter::CreateRootConstant(UINT numValues, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisiblity)
{
	if (DWORDRemaining - numValues < 0)
		assert(false);
	D3D12_ROOT_PARAMETER param;
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	param.Constants.Num32BitValues = numValues;
	param.Constants.ShaderRegister = shaderRegister;
	param.Constants.RegisterSpace = registerSpace;
	param.ShaderVisibility = shaderVisiblity;
	DWORDRemaining -= numValues;

	m_parameters.push_back(param);
}
