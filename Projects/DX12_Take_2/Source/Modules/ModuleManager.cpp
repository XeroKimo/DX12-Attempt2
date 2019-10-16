#include "PCH.h"
#include "Modules/ModuleManager.h"

bool ModuleManager::RegisterModule(IModule* module)
{
    auto it = m_modules.find(module->GetModuleType());
    if (it != m_modules.end())
        return false;

    m_modules[module->GetModuleType()] = module;
    module->OnModuleRegisterChanged(this);
    return true;
}
