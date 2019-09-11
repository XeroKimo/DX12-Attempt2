#pragma once
#include "DX12Header.h"

class DX12HRootParameter
{
	void CreateRootConstant(UINT numValues, UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_ALL);

	UINT GetParamterCount() { return static_cast<UINT>( m_parameters.size()); }
	D3D12_ROOT_PARAMETER* GetParameters() { return m_parameters.data(); }
private:
	std::vector<D3D12_ROOT_PARAMETER> m_parameters;
	UINT DWORDRemaining = 64;
};