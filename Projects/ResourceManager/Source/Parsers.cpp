#include "Parsers.h"
#include <wincodec.h>
#include <assert.h>
#include <wrl/client.h>

void ParseImage(std::wstring fileName, std::unique_ptr<unsigned char[]>& outImageData, unsigned int& outImageHeight, unsigned int& outImageWidth)
{
    using Microsoft::WRL::ComPtr;
    
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

    ComPtr<IWICBitmapFlipRotator> wicFlipper;
    wicFactory->CreateBitmapFlipRotator(wicFlipper.GetAddressOf());
    wicFlipper->Initialize(wicConverter.Get(), WICBitmapTransformFlipVertical);

    hr = wicFlipper->GetSize(&outImageWidth, &outImageHeight);
    if (FAILED(hr))
        assert(false);

    UINT stride = (outImageWidth * 4);
    UINT buffersize = stride * outImageHeight;

    outImageData = std::make_unique<unsigned char[]>(buffersize);

    hr = wicFlipper->CopyPixels(nullptr, stride, buffersize, outImageData.get());
    if (FAILED(hr))
        assert(false);
}
