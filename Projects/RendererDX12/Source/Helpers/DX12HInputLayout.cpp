#include "RendererDX12.h"
#include "Helpers/DX12HInputLayout.h"

DX12HInputLayout::DX12HInputLayout()
{
	
}

void DX12HInputLayout::AddElement(std::string semanticName, DXGI_FORMAT format, unsigned int alignedByteOffset, unsigned int semanticIndex, unsigned int inputSlot, D3D12_INPUT_CLASSIFICATION inputSlotClass, unsigned int instanceDataStepRate)
{
	D3D12_INPUT_ELEMENT_DESC desc;
	desc.SemanticName = semanticName.c_str();
	desc.Format = format;
	desc.AlignedByteOffset = alignedByteOffset;
	desc.SemanticIndex = semanticIndex;
	desc.InputSlot = inputSlot;
	desc.InputSlotClass = inputSlotClass;
	desc.InstanceDataStepRate = instanceDataStepRate;

	m_inputElements.push_back(desc);
}
