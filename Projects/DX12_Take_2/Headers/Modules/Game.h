#pragma once
#include <WinApplication.h>
#include "ModuleManager.h"


struct cBuffer { Matrix4x4 worldMatrix; Matrix4x4 viewMatrix; Matrix4x4 projMatrix; };

class EventGame : public WinApplication::IEvent
{
    std::type_index GetBaseTypeIndex() override final { return typeid(this); }
};

class EventGame2 : public WinApplication::IEvent
{
    std::type_index GetBaseTypeIndex() override final { return typeid(this); }
};

class Game : public WinApplication::IApp, public IModule, public WinApplication::IEventListener<EventGame>, public WinApplication::IEventListener<EventGame2>
{
private:
    ModuleManager* m_moduleManager = nullptr;
    WinApplication::EventManager* m_eventManager = nullptr;
    
    RendererDX12::Texture m_texture = { };
    RendererDX12::Mesh m_mesh = { };
    unique_ptr<RendererDX12::PipelineState> m_pipelineState = { };
    cBuffer buffer;
public:
    Game();

    // Inherited via IApp
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    // Inherited via IModule
    virtual void OnModuleRegisterChanged(ModuleManager* moduleManager) override;
    virtual std::type_index GetHashKey() override final { return typeid(this); };

    // Inherited via IEventListener
    virtual void OnEvent(EventGame* pEvent) override final;
    virtual void OnEvent(EventGame2* pEvent) override final;
private:
    void CreateDefaults();



};