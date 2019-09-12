#pragma once
#include "DX12Header.h"

class DX12HInputLayout
{
public:
	DX12HInputLayout();

	void AddElement(const char* semanticName, DXGI_FORMAT format, unsigned int alignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT, unsigned int semanticIndex = 0, unsigned int inputSlot = 0, D3D12_INPUT_CLASSIFICATION inputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, unsigned int instanceDataStepRate = 0);

	inline D3D12_INPUT_LAYOUT_DESC GetLayout() { D3D12_INPUT_LAYOUT_DESC layout = { m_inputElements.data(),static_cast<UINT>(m_inputElements.size()) }; return layout; }
private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputElements;
};