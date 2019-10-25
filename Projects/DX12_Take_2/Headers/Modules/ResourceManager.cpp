#include "PCH.h"
#include "ResourceManager.h"

void ResourceManager::OnModuleRegisterChanged(ModuleManager* moduleManager)
{
}

std::type_index ResourceManager::GetHashKey()
{
    return typeid(this);
}
