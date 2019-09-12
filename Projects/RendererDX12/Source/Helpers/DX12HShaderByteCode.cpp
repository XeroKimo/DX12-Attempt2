#include "RendererDX12.h"
#include "Helpers/DX12HShaderByteCode.h"

DX12HShaderByteCode::DX12HShaderByteCode()
{
}

D3D12_SHADER_BYTECODE DX12HShaderByteCode::GetByteCode()
{
	D3D12_SHADER_BYTECODE byteCode;
	byteCode.BytecodeLength = m_shaderBlob->GetBufferSize();
	byteCode.pShaderBytecode = m_shaderBlob->GetBufferPointer();
	return byteCode;
}
