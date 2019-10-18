#pragma once
#include <unordered_map>
#include <typeindex>

class ModuleManager;

__interface IModule
{
    void OnModuleRegisterChanged(ModuleManager* moduleManager);
    std::type_index GetHashKey();
};

class ModuleManager
{
private:
    std::unordered_map<std::type_index, IModule*> m_modules;
public:
    bool RegisterModule(IModule* module)
    {
        auto it = m_modules.find(module->GetHashKey());
        if (it != m_modules.end())
            return false;

        m_modules[module->GetHashKey()] = module;
        module->OnModuleRegisterChanged(this);
        return true;
    }

    template<class T>
    T* GetModule() 
    {
#if _DEBUG
        return dynamic_cast<T*>(m_modules[typeid(T)]);
#else
        return static_cast<T*>(m_modules[typeid(T)]);
#endif
    }
};