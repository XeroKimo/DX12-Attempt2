#pragma once
#include <unordered_map>

enum class ModuleType
{
    Game,
    Renderer,
    Application,
};

class ModuleManager;

__interface IModule
{
    void OnModuleRegisterChanged(ModuleManager* moduleManager);
    const ModuleType GetModuleType();
};

class ModuleManager
{
private:
    std::unordered_map<ModuleType, IModule*> m_modules;
public:
    bool RegisterModule(IModule* module);

    template<class T, ModuleType type>
    T* GetModule() 
    {
#if _DEBUG
        return dynamic_cast<T*>(m_modules[type]);
#else
        return reinterpret_cast<T*>(m_modules[type]);
#endif
    }
};