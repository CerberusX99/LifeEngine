#pragma once
#include "Prerequisites.h"

// Device class responsible for handling the Direct3D device and related resources
class Device {
public:
    // Default constructor and destructor
    Device() = default;
    ~Device() = default;

    void init();

    void update();

    void render();

    void Destroy();


    // Create a render target view (RTV) for a resource
    HRESULT CreateRenderTargetView(ID3D11Resource* pResource,
        const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
        ID3D11RenderTargetView** ppRTView);

    // Create a 2D texture resource
    HRESULT CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
        const D3D11_SUBRESOURCE_DATA* pInitialData,
        ID3D11Texture2D** ppTexture2D);

    // Create a depth-stencil view (DSV) for a resource
    HRESULT CreateDepthStencilView(ID3D11Resource* pResource,
        const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
        ID3D11DepthStencilView** ppDepthStencilView);

    // Create a vertex shader object from compiled shader bytecode
    HRESULT CreateVertexShader(const void* pShaderBytecode,
        SIZE_T BytecodeLength,
        ID3D11ClassLinkage* pClassLinkage,
        ID3D11VertexShader** ppVertexShader);

    // Create an input layout object, which describes the input data for vertex shaders
    HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
        unsigned int NumElements,
        const void* pShaderBytecodeWithInputSignature,
        unsigned int BytecodeLength,
        ID3D11InputLayout** ppInputLayout);

    // Create a pixel shader object from compiled shader bytecode
    HRESULT CreatePixelShader(const void* pShaderBytecode,
        SIZE_T BytecodeLength,
        ID3D11ClassLinkage* pClassLinkage,
        ID3D11PixelShader** ppPixelShader);

    // Create a buffer resource
    HRESULT CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
        const D3D11_SUBRESOURCE_DATA* pInitialData,
        ID3D11Buffer** ppBuffer);

    // Create a sampler state object, which defines how texture sampling operations are performed
    HRESULT CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
        ID3D11SamplerState** ppSamplerState);

public:
    ID3D11Device* m_device = nullptr; // Direct3D device pointer
};
