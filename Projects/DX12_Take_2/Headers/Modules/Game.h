#pragma once
#include <WinApplication.h>
#include "ModuleManager.h"


struct cBuffer { Matrix4x4 worldMatrix; Matrix4x4 viewMatrix; Matrix4x4 projMatrix; };

__interface IEventListenerGame : public WinApplication::IEventListener
{
    void OnEvent(WinApplication::IEvent* pEvent);
};

class EventGame : public WinApplication::IEvent
{

    // Inherited via IEvent
    virtual const std::type_index GetHashKey() override final { return typeid(*this); }
};

class Game : public WinApplication::IApp, public IModule, public IEventListenerGame
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
    virtual std::type_index GetHashKey() override final { return typeid(*this); };

    // Inherited via IEventListenerGame
    virtual void IEventListenerGame::OnEvent(WinApplication::IEvent* pEvent) override;
private:
    void CreateDefaults();

};