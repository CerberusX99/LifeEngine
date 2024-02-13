#pragma once
#include "Prerequisites.h" 

class Device; // Forward declaration of Device class
class DeviceContext; // Forward declaration of DeviceContext class

// DepthStencilView class responsible for managing the depth stencil view
class DepthStencilView
{
public:
    // Default constructor and destructor
    DepthStencilView() = default;
    ~DepthStencilView() { SAFE_RELEASE(m_depthStencilView); }

    // Initialize the depth stencil view with the specified device, depth stencil resource, and format
    void init(Device device, ID3D11Resource* depthStencil, DXGI_FORMAT format);

    void update();

    void destroy();

    // Render using the depth stencil view
    void render(DeviceContext& deviceContext);

public:
    ID3D11DepthStencilView* m_depthStencilView = nullptr; // Depth stencil view pointer
};
