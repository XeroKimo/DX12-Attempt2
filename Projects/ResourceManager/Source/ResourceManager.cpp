#include "ResourceManager.h"
#include <fstream>

void ResourceManager::Unload(std::string filePath)
{
    if (m_loadedResources.find(filePath) != m_loadedResources.end())
        m_loadedResources.erase(filePath);
}

bool ResourceManager::MetaFileExists(std::string filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return false;
    file.close();
    return true;
}
