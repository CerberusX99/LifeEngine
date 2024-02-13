#pragma once
#include "Prerequisites.h"
class Device;
class DeviceContext;
class Window;
class Texture;

// SwapChain class responsible for managing the DXGI swap chain
class SwapChain {
public:
    // Default constructor and destructor
    SwapChain() = default;
    ~SwapChain() { SAFE_RELEASE(m_swapChain); };

    // Initialize the swap chain
    void init(Device& device,
        DeviceContext& deviceContext,
        Texture& backBuffer,
        Window window);

    // Update the swap chain (if needed)
    void update();

    // Render using the swap chain
    void render();

    // Destroy the swap chain and release associated resources
    void destroy();

public:
    IDXGISwapChain* m_swapChain = nullptr; // DXGI swap chain pointer

private:
    D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL; // Driver type
    D3D_FEATURE_LEVEL m_feature_level = D3D_FEATURE_LEVEL_11_0; // DirectX feature level
};
