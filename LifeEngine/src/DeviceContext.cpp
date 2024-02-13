#include "DeviceContext.h"

// Clear the depth stencil view with the specified clear flags, depth, and stencil values
void DeviceContext::ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
    unsigned int ClearFlags,
    float Depth,
    unsigned int Stencil) {
    // Check if the depth stencil view is valid
    if (pDepthStencilView == nullptr) {
        WARNING("ERROR: DeviceContext::ClearDepthStencilView : Error in data from params [CHECK FOR ID3D11DepthStencilView* pDepthStencilView]\n");
        exit(1);
    }
    else {
        // Call the underlying Direct3D device context method to clear the depth stencil view
        m_deviceContext->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
    }
}

// Set pixel shader resources to be used by the pixel shader pipeline
void DeviceContext::PSSetShaderResources(unsigned int StartSlot,
    unsigned int NumViews,
    ID3D11ShaderResourceView* const* ppShaderResourceViews) {
    // Check if the shader resource views are valid
    if (ppShaderResourceViews == nullptr) {
        WARNING("ERROR: DeviceContext::ClearDepthStencilView : Error in data from params [CHECK FOR ID3D11ShaderResourceView* const* ppShaderResourceViews]\n");
        exit(1);
    }
    else {
        // Set the pixel shader resources using the underlying Direct3D device context method
        m_deviceContext->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
}
