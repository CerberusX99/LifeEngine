//--------------------------------------------------------------------------------------
// File: LifeEngine.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "Prerequisites.h"


#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "DepthStencilView.h"
#include "Texture.h"
#include "SwapChain.h"
#include "RenderTargetView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "ModelLoader.h"




//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
/*
struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
};*/

struct CBNeverChanges
{
    XMMATRIX mView;
};

struct CBChangeOnResize
{
    XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
Window                              g_window;
Device                              g_device;
DeviceContext                       g_deviceContext;
DepthStencilView                    g_depthStencilView;
Texture                             g_depthStencil;
SwapChain                           g_swapchain;
Texture                             g_backBuffer;
RenderTargetView                    g_renderTargetView;
Viewport                            g_viewport;
ShaderProgram                       g_shaderProgram;
Buffer                              g_vertexBuffer;
Buffer                              g_indexBuffer;
ID3D11Buffer* g_pCBNeverChanges = nullptr;
ID3D11Buffer* g_pCBChangeOnResize = nullptr;
ID3D11Buffer* g_pCBChangesEveryFrame = nullptr;
ID3D11ShaderResourceView* g_pTextureRV = nullptr;
SamplerState                        g_sampler;
ModelLoader                         g_loader;



XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
Mesh                                g_mesh;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void update();
void Render();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(g_window.init(hInstance, nCmdShow, WndProc)))
        return 0;

    if (FAILED(InitDevice()))
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render();
            update();
        }
    }

    CleanupDevice();

    return (int)msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, nullptr, ppBlobOut, &pErrorBlob, nullptr);
    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        if (pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;


    g_swapchain.init(g_device, g_deviceContext, g_backBuffer, g_window);


    // Create depth stencil texture
    g_renderTargetView.init(g_device,
        g_backBuffer,
        DXGI_FORMAT_R8G8B8A8_UNORM);


    //Create depth Stencil Texture
    g_depthStencil.init(g_device,
        g_window.m_width,
        g_window.m_height,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        D3D11_BIND_DEPTH_STENCIL);

    // Create the depth stencil view

    g_depthStencilView.init(g_device,
        g_depthStencil.m_texture,
        DXGI_FORMAT_D24_UNORM_S8_UINT);


    //g_deviceContext.m_deviceContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_depthStencilView.m_depthStencilView );

    g_viewport.init(g_window);

    // Setup the viewport


    // Compile the vertex shader



    // Define the input layout


    //UINT numElements = ARRAYSIZE( layout );

    std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
    D3D11_INPUT_ELEMENT_DESC position;
    position.SemanticName = "POSITION";
    position.SemanticIndex = 0;
    position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    position.InputSlot = 0;
    position.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT /*12*/;
    position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    position.InstanceDataStepRate = 0;
    Layout.push_back(position);

    D3D11_INPUT_ELEMENT_DESC texcoord;
    texcoord.SemanticName = "TEXCOORD";
    texcoord.SemanticIndex = 0;
    texcoord.Format = DXGI_FORMAT_R32G32_FLOAT;
    texcoord.InputSlot = 0;
    texcoord.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT /*12*/;
    texcoord.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    texcoord.InstanceDataStepRate = 0;
    Layout.push_back(texcoord);

    //Init Shader
    g_shaderProgram.init(g_device, "LifeEngine.fx", Layout);


    //Generar Mesh
    //->Carga de modelo 3D
    //Create Vertex buffer

    SimpleVertex vertices[] =
    {
         { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
    };

    g_mesh = g_loader.Load("norm.obj");
  //  for (SimpleVertex vertex : vertices)
    //{
      //  g_mesh.vertex.push_back(vertex);
    //}
    //g_mesh.numVertex = g_mesh.vertex.size();

    // Create vertex buffer
    g_vertexBuffer.init(g_device, g_mesh, D3D11_BIND_VERTEX_BUFFER);
    
 
   

    unsigned int indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

   //for (unsigned int index : indices)
    //{
   //     g_mesh.index.push_back(index);
    //}
    //g_mesh.numIndex = g_mesh.index.size();

    // Create index buffer

    g_indexBuffer.init(g_device, g_mesh, D3D11_BIND_INDEX_BUFFER);



    // Create the constant buffers
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBNeverChanges);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_device.CreateBuffer(&bd, nullptr, &g_pCBNeverChanges);
    if (FAILED(hr))
        return hr;

    bd.ByteWidth = sizeof(CBChangeOnResize);
    hr = g_device.CreateBuffer(&bd, nullptr, &g_pCBChangeOnResize);
    if (FAILED(hr))
        return hr;

    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = g_device.CreateBuffer(&bd, nullptr, &g_pCBChangesEveryFrame);
    if (FAILED(hr))
        return hr;

    // Load the Texture
    hr = D3DX11CreateShaderResourceViewFromFile(g_device.m_device, "seafloor.dds", nullptr, nullptr, &g_pTextureRV, nullptr);
    if (FAILED(hr))
        return hr;

    // Create the sample state
    g_sampler.init(g_device);

    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    g_View = XMMatrixLookAtLH(Eye, At, Up);

    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = XMMatrixTranspose(g_View);
    g_deviceContext.m_deviceContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbNeverChanges, 0, 0);

    // Initialize the projection matrix
    g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_window.m_width / (FLOAT)g_window.m_height, 0.01f, 100.0f);

    CBChangeOnResize cbChangesOnResize;
    cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
    g_deviceContext.m_deviceContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangesOnResize, 0, 0);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    //Release device context
    g_deviceContext.destroy();
    //Release Sample Linear
    g_sampler.destroy();
    //Release ModelTextures
    if (g_pTextureRV) g_pTextureRV->Release();
    //Releas Camera resources
    if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
    if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
    //Release Model resources
    if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
    // Release Shader Resources
    //if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    //if( g_pIndexBuffer ) g_pIndexBuffer->Release();
    g_vertexBuffer.destroy();
    g_vertexBuffer.destroy();
    //if( g_pVertexLayout ) g_pVertexLayout->Release();
    //if( g_pVertexShader ) g_pVertexShader->Release();
    //if( g_pPixelShader ) g_pPixelShader->Release();
    //Release depth stencil
    g_shaderProgram.destroy();
    g_depthStencil.destroy();

    //Release depth stencil view
    g_depthStencilView.destroy();
    //Release render target view
    //if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    g_renderTargetView.destroy();
    //Release swapchain
    g_swapchain.destroy();
    //Release UI
    //Release device 
    g_device.destroy();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------

void update()
{
    // Update our time
    static float t = 0.0f;
    if (g_swapchain.m_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // Rotate cube around the origin
    g_World = XMMatrixRotationY(t);

    // Modify the color
    g_vMeshColor.x = 1.0f;//( sinf( t * 1.0f ) + 1.0f ) * 0.5f;
    g_vMeshColor.y = 1.0f;//( cosf( t * 3.0f ) + 1.0f ) * 0.5f;
    g_vMeshColor.z = 1.0f;//( sinf( t * 5.0f ) + 1.0f ) * 0.5f;

}


void Render()
{

    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha





    g_depthStencilView.render(g_deviceContext);
    g_renderTargetView.render(g_deviceContext, g_depthStencilView, 1, ClearColor);

    //set Viewport
    g_viewport.render(g_deviceContext);

    //
    // Update variables that change once per frame
    //
    CBChangesEveryFrame cb;
    cb.mWorld = XMMatrixTranspose(g_World);
    cb.vMeshColor = g_vMeshColor;
    g_deviceContext.m_deviceContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    //
    // Render the cube
    //
    g_shaderProgram.render(g_deviceContext);
    g_deviceContext.m_deviceContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
    g_deviceContext.m_deviceContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
    g_deviceContext.m_deviceContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
    g_deviceContext.m_deviceContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);


    g_sampler.render(g_deviceContext, 0, 1);
    g_vertexBuffer.render(g_deviceContext, 0, 1);
    g_indexBuffer.render(g_deviceContext, DXGI_FORMAT_R32_UINT);

    g_deviceContext.m_deviceContext->PSSetShaderResources(0, 1, &g_pTextureRV);

    g_deviceContext.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_deviceContext.m_deviceContext->DrawIndexed(g_mesh.numIndex, 0, 0);

    //
    // Present our back buffer to our front buffer
    //
    g_swapchain.present();
}
