#pragma once
#include "Level 0/BaseDevice.h"
#include "Level 1/BaseSwapChain.h"
#include "Level 3/ManagerConstantBuffer.h"
#include "Level 3/ManagerCommandAllocator.h"
#include "Level 4/DeviceCommandModule.h"
#include <WinApplication.h>
#include "ModuleManager.h"

class Renderer : public IModule
{
private:
    ModuleManager* m_moduleManager = nullptr;

    std::unique_ptr<RendererDX12::BaseDevice> m_device;
    std::unique_ptr<RendererDX12::BaseSwapChain> m_swapChain;
    std::unique_ptr<RendererDX12::ManagerConstantBuffer> m_constantBufferManager;
    std::unique_ptr<RendererDX12::ManagerCommandAllocator> m_commandAllocatorManager;
    std::unique_ptr<RendererDX12::DeviceCommandModule> m_deviceCommandModule;

public:
    Renderer();
    bool Initialize(WinApplication::Window* window);

    RendererDX12::BaseDevice* GetDevice() { return m_device.get(); }
    RendererDX12::BaseSwapChain* GetSwapChain() { return m_swapChain.get(); }
    RendererDX12::ManagerConstantBuffer* GetConstantBufferManager() { return m_constantBufferManager.get(); }
    RendererDX12::ManagerCommandAllocator* GetCommandAllocatorManager() { return m_commandAllocatorManager.get(); }
    RendererDX12::DeviceCommandModule* GetDeviceCommandModule() { return m_deviceCommandModule.get(); }

    void OnWindowSizeChange(WPARAM wParam, LPARAM lParam);

    // Inherited via IModule
    virtual void OnModuleRegisterChanged(ModuleManager* moduleManager) override;
    virtual std::type_index GetHashKey() override final { return typeid(this); };


};