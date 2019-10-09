#pragma once
#include "DX12Header.h"

namespace RendererDX12
{
    struct DX12HShaderByteCode
    {
    public:
        DX12HShaderByteCode();

        inline HRESULT CompileShaderFromFile(LPCWSTR fileName, LPCSTR shaderVersion, LPCSTR entryPoint = "main", const D3D_SHADER_MACRO* pDefines = nullptr, ID3DInclude* pIncludes = nullptr, UINT flags1 = 0, UINT flags2 = 0) {
            return D3DCompileFromFile(fileName, pDefines, pIncludes, entryPoint, shaderVersion, flags1, flags2, &m_shaderBlob, &m_errorBlob);
        }

        D3D12_SHADER_BYTECODE GetByteCode();
        inline ID3DBlob* GetError() { return m_errorBlob.Get(); }
    private:
        ComPtr<ID3DBlob> m_shaderBlob;
        ComPtr<ID3DBlob> m_errorBlob;
    };
}