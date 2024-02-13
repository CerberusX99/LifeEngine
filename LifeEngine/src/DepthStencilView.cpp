#include "DepthStencilView.h"
#include "Device.h"
#include "DeviceContext.h"

// Initialize the DepthStencilView with the specified device, depthStencil resource, and format
void DepthStencilView::init(Device device, ID3D11Resource* depthStencil, DXGI_FORMAT format) {
    // Check if the device or depthStencil resource is valid
    if (device.m_device == nullptr) {
        WARNING("ERROR: Device::DepthStencilView::init : Error in data from params [CHECK FOR Device device]\n");
        exit(1);
    }
    else if (depthStencil == nullptr) {
        WARNING("ERROR: Device::DepthStencilView::init : Error in data from params [CHECK FOR ID3D11Resource* depthStencil]\n");
        exit(1);
    }
    else {
        // Create the depth stencil view with the specified format and dimension
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        memset(&descDSV, 0, sizeof(descDSV));
        descDSV.Format = format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        device.CreateDepthStencilView(depthStencil, &descDSV, &m_depthStencilView);
    }
}

void DepthStencilView::update() {
   
}

// Render using the DepthStencilView by clearing the depth stencil buffer
void DepthStencilView::render(DeviceContext& deviceContext) {
    // Clear the depth stencil view with the specified clear values
    deviceContext.ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

// Destroy the DepthStencilView and release associated resources
void DepthStencilView::destroy() {
    // Release the depth stencil view
    SAFE_RELEASE(m_depthStencilView);
}
