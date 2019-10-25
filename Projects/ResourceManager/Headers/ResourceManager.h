#pragma once
#include <memory>
#include <unordered_map>
#include <string>

const std::string fileAffix = ".rec";
const std::string rootDir = "Resources/";
class ResourceManager;
class Resource
{
protected:
    virtual bool GenerateMetaFile(std::string filePath, std::string metaFileAffix) = 0;
    virtual void LoadMetaFile(std::string filePath) = 0;
    friend class ResourceManager;
};

class ResourceManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_loadedResources;

public:
    template<class T, std::enable_if_t<std::is_base_of_v<Resource,T>>>
    std::shared_ptr<T> Load(std::string filePath);

    void Unload(std::string filePath);

private:
    bool MetaFileExists(std::string filePath);

    template<class T, std::enable_if_t<std::is_base_of_v<Resource, T>>>
    std::shared_ptr<T> GenerateMetaFile(std::string filePath);

    template<class T, std::enable_if_t<std::is_base_of_v<Resource, T>>>
    std::shared_ptr<T> LoadMetaFile(std::string filePath);
};


template<class T, std::enable_if_t<std::is_base_of_v<Resource, T>>>
std::shared_ptr<T> ResourceManager::Load(std::string filePath)
{
    if (m_loadedResources.find(filePath) != m_loadedResources.end())
        return m_loadedResources[filePath];

    if (!MetaFileExists(filePath))
        return GenerateMetaFile<T>(filePath);

    return LoadMetaFile<T>(filePath);
}

template<class T, std::enable_if_t<std::is_base_of_v<Resource, T>>>
std::shared_ptr<T> ResourceManager::GenerateMetaFile(std::string filePath)
{
    std::shared_ptr<T> fileToMake = std::make_shared<T>();
    if (!fileToMake->GenerateMetaFile(filePath + fileAffix))
        return nullptr;

    m_loadedResources[filePath] = fileToMake;
    return fileToMake;
}

template<class T, std::enable_if_t<std::is_base_of_v<Resource, T>>>
std::shared_ptr<T> ResourceManager::LoadMetaFile(std::string filePath)
{
    std::shared_ptr<T> fileToLoad = std::make_shared<T>();
    fileToLoad->LoadMetaFile(filePath + fileAffix);
    m_loadedResources[filePath] = fileToLoad;
    return fileToLoad;
}