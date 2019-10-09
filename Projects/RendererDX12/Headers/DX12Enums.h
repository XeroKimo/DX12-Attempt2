#pragma once

namespace RendererDX12
{
    enum class Command_List_Type
    {
        Direct = D3D12_COMMAND_LIST_TYPE_DIRECT,
        Bundle = D3D12_COMMAND_LIST_TYPE_BUNDLE,
        Compute = D3D12_COMMAND_LIST_TYPE_COMPUTE,
        Copy = D3D12_COMMAND_LIST_TYPE_COPY,
        Video_Decode = D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE,
        Video_Process = D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS
    };
}