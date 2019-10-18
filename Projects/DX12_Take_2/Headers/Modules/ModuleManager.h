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

    template<class T, class = std::enable_if_t<std::is_base_of_v<IModule,T>>>
    bool RegisterModule(IModule* module)
    {
        if (std::type_index(typeid(T*)) != module->GetHashKey())
            return false;

        auto it = m_modules.find(typeid(T*));
        if (it != m_modules.end())
            return false;

        m_modules[typeid(T*)] = module;
        module->OnModuleRegisterChanged(this);
        return true;
    }

    template<class T>
    T* GetModule() 
    {
#if _DEBUG
        return dynamic_cast<T*>(m_modules[typeid(T*)]);
#else
        return static_cast<T*>(m_modules[typeid(T*)]);
#endif
    }
};