#pragma once
#include "Prerequisites.h"

//Foward declaretin
class Device;
class DeviceContext;
class Texture;
class DepthStencilView;

class
    RenderTargetView
{
public:
    RenderTargetView() = default;
    ~RenderTargetView() {};

    void
        init(Device device, Texture backBuffer, DXGI_FORMAT Format);

    void
        update();

    void
        render(DeviceContext& deviceContext,
            DepthStencilView& depthStencilView,
            unsigned int numViews,
            float ClearColor[4]);

    void
        destroy();

public:
    ID3D11RenderTargetView* m_renderTargetView = nullptr;
    float m_cleanColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
};
