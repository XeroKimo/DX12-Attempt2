#pragma once
#include <WinApplication.h>
#include "ModuleManager.h"


struct cBuffer { Matrix4x4 worldMatrix; Matrix4x4 viewMatrix; Matrix4x4 projMatrix; };

class EventGame : public WinApplication::IMessage
{
    std::type_index GetBaseTypeIndex() override final { return typeid(this); }
};

class EventGame2 : public WinApplication::IMessage
{
    std::type_index GetBaseTypeIndex() override final { return typeid(this); }
};

class Game : public WinApplication::IApp, public IModule
{
private:
    ModuleManager* m_moduleManager = nullptr;
    WinApplication::MessageManager* m_eventManager = nullptr;
    
    RendererDX12::Texture m_texture = { };
	RendererDX12::Texture m_houseTexture = {};
    RendererDX12::Mesh m_mesh = { };
	RendererDX12::Mesh m_house = { };
    std::unique_ptr<RendererDX12::PipelineState> m_pipelineState = { };
    cBuffer buffer;
    cBuffer buffer2;
	cBuffer buffer3;

    Matrix4x4 camera;

    bool aPressed;
    bool dPressed;
    bool wPressed;
    bool sPressed;
public:
    Game();

    // Inherited via IApp
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    // Inherited via IModule
    virtual void OnModuleRegisterChanged(ModuleManager* moduleManager) override;
    virtual std::type_index GetHashKey() override final { return typeid(this); };
    
    void OnEvent(EventGame* event);

    void OnWindowDestory(WPARAM wParam, LPARAM lParam);
    void RotateCamera(WPARAM wParam, LPARAM lParam);
    void StopRotate(WPARAM wParam, LPARAM lParam);
private:
    void CreateDefaults();



};