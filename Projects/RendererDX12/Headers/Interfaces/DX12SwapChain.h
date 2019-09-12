#pragma once
#include "DX12Header.h"

class DX12Device;
class DX12SwapChain
{
public:
	DX12SwapChain();
	void Initialize(ID3D12Device* device, ID3D12CommandQueue* commandQueue, HWND windowHandle, UINT windowWidth, UINT windowHeight);

	void ClearBackBuffer(shared_ptr<DX12CommandList>& commandList);
	inline IDXGISwapChain3* GetSwapChain() { return m_swapChain.Get(); }
private:
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12DescriptorHeap> m_renderTargetHeap;
	std::vector<ComPtr<ID3D12Resource1>> m_frameBuffers;
	D3D12_VIEWPORT m_viewPort;
	D3D12_RECT m_rect;
	UINT m_descriptorHeapSize;
};