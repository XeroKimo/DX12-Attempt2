#include "RendererDX12.h"
#include "Helpers/ShaderInputLayout.h"

namespace RendererDX12
{
    namespace Helpers
    {
        ShaderInputLayout::ShaderInputLayout()
        {

        }

        void ShaderInputLayout::AddElement(const char* semanticName, DXGI_FORMAT format, unsigned int alignedByteOffset, unsigned int semanticIndex, unsigned int inputSlot, D3D12_INPUT_CLASSIFICATION inputSlotClass, unsigned int instanceDataStepRate)
        {
            D3D12_INPUT_ELEMENT_DESC desc;
            desc.SemanticName = semanticName;
            desc.Format = format;
            desc.AlignedByteOffset = alignedByteOffset;
            desc.SemanticIndex = semanticIndex;
            desc.InputSlot = inputSlot;
            desc.InputSlotClass = inputSlotClass;
            desc.InstanceDataStepRate = instanceDataStepRate;

            m_inputElements.push_back(desc);
        }
    }
}