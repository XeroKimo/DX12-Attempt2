#include "RendererDX12.h"
#include "Level 2/DX12Texture.h"
#include <wincodec.h>

namespace RendererDX12
{
    void DX12Texture::InitializeTexture2D(ID3D12Device* device, DX12CommandList* commandList, std::wstring filename)
    {
        unique_ptr<BYTE[]> imageData;
        unsigned int imageWidth;
        unsigned int imageHeight;

        ParseImage(filename, imageData, imageWidth, imageHeight);

        device->CreateCommittedResource(&DX12HResource::HeapDefault(0), D3D12_HEAP_FLAG_NONE, &DX12HResource::Texture2D(imageWidth, imageHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 1), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()));

        D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
        heapDesc.NodeMask = 0;
        heapDesc.NumDescriptors = 1;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_heap.GetAddressOf()));
        assert(SUCCEEDED(hr));

        m_resourceView.Format = m_resource.Get()->GetDesc().Format;
        m_resourceView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        m_resourceView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        m_resourceView.Texture2D.MipLevels = 1;
        m_resourceView.Texture2D.MostDetailedMip = 0;
        m_resourceView.Texture2D.PlaneSlice = 0;
        m_resourceView.Texture2D.ResourceMinLODClamp = 0.0f;

        device->CreateShaderResourceView(m_resource.Get(), &m_resourceView, m_heap->GetCPUDescriptorHandleForHeapStart());
        D3D12_SUBRESOURCE_DATA data;
        data.pData = imageData.get();
        data.RowPitch = static_cast<LONG_PTR>(imageWidth) * 4;
        data.SlicePitch = imageHeight * data.RowPitch;

        commandList->UploadData(m_resource.Get(), &data);

        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_resource.Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        //barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        commandList->GetInterface()->ResourceBarrier(1, &barrier);
    }

    void DX12Texture::Set(DX12CommandList* commandList, const UINT& paramIndex)
    {
        commandList->GetInterface()->SetDescriptorHeaps(1, m_heap.GetAddressOf());
        commandList->GetInterface()->SetGraphicsRootDescriptorTable(paramIndex, m_heap->GetGPUDescriptorHandleForHeapStart());
    }

    void DX12Texture::ParseImage(std::wstring fileName, unique_ptr<BYTE[]>& outImageData, unsigned int& outImageWidth, unsigned int& outImageHeight)
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

        UINT stride = (outImageWidth * 4);
        UINT buffersize = stride * outImageHeight;

        outImageData = make_unique<BYTE[]>(buffersize);

        ComPtr<IWICBitmapFlipRotator> wicFlipper;
        wicFactory->CreateBitmapFlipRotator(wicFlipper.GetAddressOf());
        wicFlipper->Initialize(wicConverter.Get(), WICBitmapTransformFlipVertical);

        hr = wicFlipper->CopyPixels(nullptr, stride, buffersize, outImageData.get());
        if (FAILED(hr))
            assert(false);
    }
}