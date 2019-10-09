#include "RendererDX12.h"
#include "Helpers/ShaderByteCode.h"

namespace RendererDX12
{
    namespace Helpers
    {
        ShaderByteCode::ShaderByteCode()
        {
        }

        D3D12_SHADER_BYTECODE ShaderByteCode::GetByteCode()
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
}