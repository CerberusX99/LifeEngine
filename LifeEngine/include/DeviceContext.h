#pragma once
#include "Prerequisites.h"

// DeviceContext class responsible for managing the Direct3D device context
class DeviceContext
{
public:
    // Default constructor and destructor
    DeviceContext() = default;
    ~DeviceContext() { SAFE_RELEASE(m_deviceContext); };

    void init();

    void update();

    void render();

    void destroy();

    // Clear the depth stencil view with the specified clear flags, depth, and stencil values
    void ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
        unsigned int ClearFlags,
        float Depth,
        unsigned int Stencil);

    // Set pixel shader resources to be used by the pixel shader pipeline
    void PSSetShaderResources(unsigned int StartSlot,
        unsigned int NumViews,
        ID3D11ShaderResourceView* const* ppShaderResourceViews);

public:
    ID3D11DeviceContext* m_deviceContext = nullptr; // Direct3D device context pointer
};
