#pragma once
#include "Prerequisites.h"

class
    DeviceContext {
public:
    DeviceContext() = default;
    ~DeviceContext() { SAFE_RELEASE(m_deviceContext); };

    void
        init();

    void
        update();

    void
        render();

    void
        destroy();

    void
        ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
            unsigned int ClearFlags,
            float Depth,
            unsigned int  Stencil);

    void
        PSSetShaderResources(unsigned int StartSlot,
            unsigned int NumViews,
            ID3D11ShaderResourceView* const* ppShaderResourceViews);

    void
        OMSetRenderTargets(unsigned int NumViews,
            ID3D11RenderTargetView* const* ppRenderTargetViews,
            ID3D11DepthStencilView* pDepthStencilView);

    void
        ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
            const float ColorRGBA[4]);

    void
        RSSetViewports(unsigned int NumViewports, const D3D11_VIEWPORT* pViewports);

    void
        IASetInputLayout(ID3D11InputLayout* pInputLayout);

    void
        VSSetShader(ID3D11VertexShader* pVertexShader,
            ID3D11ClassInstance* const* ppClassInstances,
            unsigned int NumClassInstances);

    void
        PSSetShader(ID3D11PixelShader* pPixelShader,
            ID3D11ClassInstance* const* ppClassInstances,
            unsigned int NumClassInstances);

    void
        UpdateSubresource(ID3D11Resource* pDstResource,
            unsigned int DstSubresource,
            const D3D11_BOX* pDstBox,
            const void* pSrcData,
            unsigned int SrcRowPitch,
            unsigned int SrcDepthPitch);

    void
        IASetVertexBuffers(unsigned int StartSlot,
            unsigned int NumBuffers,
            ID3D11Buffer* const* ppVertexBuffers,
            const unsigned int* pStrides,
            const unsigned int* pOffsets);

    void
        IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
            DXGI_FORMAT Format,
            unsigned int Offset);

    void
        PSSetSamplers(unsigned int StartSlot,
            unsigned int NumSamplers,
            ID3D11SamplerState* const* ppSamplers);
public:
    ID3D11DeviceContext* m_deviceContext = nullptr;
};