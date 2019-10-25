#pragma once
#include "ModuleManager.h"

class ResourceManager : public IModule
{
private:
    ModuleManager* m_moduleManager;
public:
    // Inherited via IModule
    virtual void OnModuleRegisterChanged(ModuleManager* moduleManager) override;
    virtual std::type_index GetHashKey() override;
};