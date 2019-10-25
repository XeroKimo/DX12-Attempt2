#pragma once
#include <string>
#include <memory>

void ParseImage(std::wstring fileName, std::unique_ptr<unsigned char[]>& outImageData, unsigned int& outImageHeight, unsigned int& outImageWidth);