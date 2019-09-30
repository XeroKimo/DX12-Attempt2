#include "RendererDX12.h"
#include "Level 2/DX12Texture.h"
#include <wincodec.h>

void DX12Texture::InitializeTexture2D(ID3D12Device* device, DX12CommandList* commandList, std::wstring filename)
{
	unique_ptr<BYTE[]> imageData;
	unsigned int imageWidth;
	unsigned int imageHeight;

	ParseImage(filename, imageData, imageWidth, imageHeight);

	D3D12_HEAP_PROPERTIES heapProperties;
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = imageWidth;
	desc.Height = imageHeight;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	HRESULT hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));
	assert(SUCCEEDED(hr));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_heap.GetAddressOf()));
	assert(SUCCEEDED(hr));

	D3D12_SUBRESOURCE_DATA data;
	data.pData = &imageData;
	data.RowPitch = 4;
	data.SlicePitch = 4 * imageHeight;

	commandList->UploadData(m_resource.Get(), &data, 0, 1);

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = m_resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	commandList->GetBase()->GetInterface()->ResourceBarrier(1, &barrier);
}

void DX12Texture::ParseImage(std::wstring fileName, unique_ptr<BYTE[]>& outImageData, unsigned int& outImageHeight, unsigned int& outImageWidth)
{
	ComPtr<IWICImagingFactory> wicFactory = nullptr;
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(wicFactory.GetAddressOf()))))
		assert(false);

	ComPtr<IWICBitmapDecoder> wicDecoder;
	HRESULT hr = wicFactory->CreateDecoderFromFilename(fileName.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, wicDecoder.GetAddressOf());
	if (FAILED(hr))
		assert(false);

	ComPtr<IWICBitmapFrameDecode> wicFrame;
	wicDecoder->GetFrame(0, wicFrame.GetAddressOf());

	ComPtr<IWICFormatConverter> wicConverter;
	wicFactory->CreateFormatConverter(wicConverter.GetAddressOf());
	hr = wicConverter->Initialize(wicFrame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 1.0, WICBitmapPaletteTypeCustom);
	if (FAILED(hr))
		assert(false);



	hr = wicFrame->GetSize(&outImageWidth, &outImageHeight);
	if (FAILED(hr))
		assert(false);

	UINT stride = outImageWidth * 32;
	UINT buffersize = stride * outImageHeight;

	outImageData = make_unique<BYTE[]>(buffersize);

	hr = wicConverter->CopyPixels(nullptr, stride, buffersize, outImageData.get());
	if (FAILED(hr))
		assert(false);
}
