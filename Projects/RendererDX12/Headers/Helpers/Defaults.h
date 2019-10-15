#pragma once

namespace RendererDX12
{
    namespace Helpers
    {
        namespace Defaults
        {
            inline D3D12_STATIC_SAMPLER_DESC StaticSamplerAnisotropic(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_PIXEL, UINT registerSpace = 0, UINT maxAnisotropy = 16, D3D12_TEXTURE_ADDRESS_MODE textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP)
            {
                D3D12_STATIC_SAMPLER_DESC desc = {};
                desc.Filter = D3D12_FILTER_ANISOTROPIC;
                desc.AddressU = desc.AddressV = desc.AddressW = textureAddressMode;
                desc.MipLODBias = 0;
                desc.MaxAnisotropy = maxAnisotropy;
                desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
                desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
                desc.MinLOD = 0;
                desc.MaxLOD = D3D12_FLOAT32_MAX;
                desc.ShaderRegister = shaderRegister;
                desc.RegisterSpace = registerSpace;
                desc.ShaderVisibility = shaderVisiblity;

                return desc;
            }

            inline D3D12_STATIC_SAMPLER_DESC StaticSamplerLinear(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_PIXEL, UINT registerSpace = 0, D3D12_TEXTURE_ADDRESS_MODE textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP)
            {
                D3D12_STATIC_SAMPLER_DESC desc = {};
                desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
                desc.AddressU = desc.AddressV = desc.AddressW = textureAddressMode;
                desc.MipLODBias = 0;
                desc.MaxAnisotropy = 0;
                desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
                desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
                desc.MinLOD = 0;
                desc.MaxLOD = D3D12_FLOAT32_MAX;
                desc.ShaderRegister = shaderRegister;
                desc.RegisterSpace = registerSpace;
                desc.ShaderVisibility = shaderVisiblity;

                return desc;
            }

            inline D3D12_STATIC_SAMPLER_DESC StaticSamplerPoint(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisiblity = D3D12_SHADER_VISIBILITY_PIXEL, UINT registerSpace = 0, D3D12_TEXTURE_ADDRESS_MODE textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP)
            {
                D3D12_STATIC_SAMPLER_DESC desc = {};
                desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
                desc.AddressU = desc.AddressV = desc.AddressW = textureAddressMode;
                desc.MipLODBias = 0;
                desc.MaxAnisotropy = 0;
                desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
                desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
                desc.MinLOD = 0;
                desc.MaxLOD = D3D12_FLOAT32_MAX;
                desc.ShaderRegister = shaderRegister;
                desc.RegisterSpace = registerSpace;
                desc.ShaderVisibility = shaderVisiblity;

                return desc;
            }

        }
    }
}