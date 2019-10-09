#include "RendererDX12.h"
#include "Helpers/DX12HShaderByteCode.h"

namespace RendererDX12
{
    DX12HShaderByteCode::DX12HShaderByteCode()
    {
    }

    D3D12_SHADER_BYTECODE DX12HShaderByteCode::GetByteCode()
    {
        D3D12_SHADER_BYTECODE byteCode;
        if (m_shaderBlob)
        {
            byteCode.BytecodeLength = m_shaderBlob->GetBufferSize();
            byteCode.pShaderBytecode = m_shaderBlob->GetBufferPointer();
        }
        else
        {
            byteCode.BytecodeLength = 0;
            byteCode.pShaderBytecode = nullptr;
        }
        return byteCode;
    }
}