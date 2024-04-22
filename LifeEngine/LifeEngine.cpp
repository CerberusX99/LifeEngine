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
#include "UserInterface.h"
#include "Transform.h"






//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
/*
struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
};*/




//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
Window                              g_window;
Device                              g_device;
DeviceContext                       g_deviceContext;
Texture                             g_depthStencil;
DepthStencilView                    g_depthStencilView;
SwapChain                           g_swapchain;
Texture                             g_backBuffer;
RenderTargetView                    g_renderTargetView;
Viewport                            g_viewport;
ShaderProgram                       g_shaderProgram;
Buffer                              g_vertexBuffer;
Buffer                              g_indexBuffer;
Buffer                              g_camera;
Buffer                              g_modelBuffer;
Buffer                              g_lightBuffer;
Texture                             g_albedo;
Texture                             g_normal;
SamplerState                        g_sampler;
UserInterface                       UI;
Transform                           g_transform;


XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
Mesh                                g_mesh;
ModelLoader                         g_loader;
Camera                              g_cameraData;
LightConfig                         g_lightConfig;
float                               g_LightMovementSpeed = 1.0f;
XMFLOAT4                            g_LightPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
// Render Model in ImGui
ID3D11Texture2D* imguiTexture;
ID3D11RenderTargetView* imguiRTV;
ID3D11ShaderResourceView* imguiSRV = nullptr;


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
            update();
            Render();
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

    // Setup the viewport
    g_viewport.init(g_window);


    // Compile the vertex shader

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

    D3D11_INPUT_ELEMENT_DESC normal;
    normal.SemanticName = "NORMAL";
    normal.SemanticIndex = 0;
    normal.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    normal.InputSlot = 0;
    normal.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; /*12*/
    normal.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    normal.InstanceDataStepRate = 0;
    Layout.push_back(normal);

    //Init Shader
    g_shaderProgram.init(g_device, "ShaderV2.fx", Layout);


    //Generar Mesh
    g_mesh = g_loader.Load("Pistol.obj");

    //Create Vertex buffer
    g_vertexBuffer.init(g_device, g_mesh, D3D11_BIND_VERTEX_BUFFER);

    // Create index buffer
    g_indexBuffer.init(g_device, g_mesh, D3D11_BIND_INDEX_BUFFER);

    // Create the constant buffers
    g_camera.init(g_device, sizeof(Camera));

    g_modelBuffer.init(g_device, sizeof(CBChangesEveryFrame));
    g_lightBuffer.init(g_device, sizeof(LightConfig));

    // Load the Texture

    g_albedo.init(g_device, "GunAlbedo.dds");
    g_normal.init(g_device, "normal.dds");

    // Create the sample state
    g_sampler.init(g_device);

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = g_window.m_width;
    textureDesc.Height = g_window.m_height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    g_device.m_device->CreateTexture2D(&textureDesc, NULL, &imguiTexture);

    // Crear una vista de render target para la textura de IMGUI
    g_device.m_device->CreateRenderTargetView(imguiTexture, NULL, &imguiRTV);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    g_device.m_device->CreateShaderResourceView(imguiTexture, &srvDesc, &imguiSRV);


    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, -30.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    g_View = XMMatrixLookAtLH(Eye, At, Up);

    g_cameraData.mView = XMMatrixTranspose(g_View);
    // Initialize the projection matrix
    g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_window.m_width / (float)g_window.m_height, 0.01f, 100.0f);

    g_cameraData.mProjection = XMMatrixTranspose(g_Projection);

    g_lightConfig.LightPos = XMFLOAT4(0.0f, 0.0f, 70.0f, 0); // Posición de la luz en el espacio 3D
    g_lightConfig.LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f); // Color de la luz en RGB
    g_lightConfig.AmbientIntensity = 0.2f; // Intensidad ambiental de la luz

    UI.init(g_window.m_hWnd, g_device.m_device, g_deviceContext.m_deviceContext);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    // Release Device Context
    g_deviceContext.destroy();
    // Release Sampler Linear
    g_sampler.destroy();
    // Release Model Textures
    g_albedo.destroy();
    g_normal.destroy();
    // Release Camera resources
    g_camera.destroy();
    // Release Model resources
    g_modelBuffer.destroy();
    g_lightBuffer.destroy();
    // Release Shader resources
    g_vertexBuffer.destroy();
    g_indexBuffer.destroy();
    // Release Shader program
    g_shaderProgram.destroy();
    // Release depth stencil
    g_depthStencil.destroy();
    // Release depth stencil view 
    g_depthStencilView.destroy();
    // Release render target view 
    g_renderTargetView.destroy();
    // Release swapchain
    g_swapchain.destroy();
    // Release UI
    // 
    // Release device
    g_device.destroy();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message

//--------------------------------------------------------------------------------------
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
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
    //Ui update

    UI.update();
    //ImGui Demo
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);


    // EASY TRANSFORM
    ImGui::Begin(" TRANSFORM");

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("POSITION");
    ImGui::PopStyleColor();
    ImGui::SliderFloat("Position X", &g_transform.m_v3Position.x, -4.0f, 1.0f);
    ImGui::SliderFloat("Position Y", &g_transform.m_v3Position.y, -2.0f, 2.0f);
    ImGui::SliderFloat("Position Z", &g_transform.m_v3Position.z, -2.0f, 2.0f);

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("ROTATION");
    ImGui::PopStyleColor();
    ImGui::SliderFloat("Rotation X", &g_transform.m_v3Rotate.x, -6.0f, 6.0f);
    ImGui::SliderFloat("Rotation Y", &g_transform.m_v3Rotate.y, -6.0f, 6.0f);
    ImGui::SliderFloat("Rotation Z", &g_transform.m_v3Rotate.z, -6.0f, 6.0f);

    ImGui::End();

    // EASY SCALE
    ImGui::Begin(" SCALE");

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("SCALE");
    ImGui::PopStyleColor();
    ImGui::SliderFloat("Rotation X", &g_transform.m_v3Scale.x, 0.4f, 1.0f);
    ImGui::SliderFloat("Rotation Y", &g_transform.m_v3Scale.y, 0.4f, 1.0f);
    ImGui::SliderFloat("Rotation Z", &g_transform.m_v3Scale.z, 0.4f, 1.0f);

    ImGui::End();

    // DATA TRANSFORM
    ImGui::Begin("DATA TRANSFORM");

    UI.vec3Control("Position", &g_transform.m_v3Position.x);
    UI.vec3Control("Rotation", &g_transform.m_v3Rotate.x);
    UI.vec3Control("Scale", &g_transform.m_v3Scale.x, 0.5f);

    ImGui::End();

    // COLOR BACKGROUND
    ImGui::Begin("COLOR BACKGROUND");
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("Color widget with Float Display:");
    ImGui::PopStyleColor();
    ImGui::ColorEdit4("MyColor##2f", (float*)&g_renderTargetView.m_cleanColor, ImGuiColorEditFlags_Float);
    ImGui::End();

    // COLOR MODEL TEXTURE
    ImGui::Begin("COLOR MODEL TEXTURE");

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("White is better:");
    ImGui::PopStyleColor();
    ImGui::ColorEdit4("MyColor##2f", (float*)&g_vMeshColor, ImGuiColorEditFlags_Float);

    ImGui::End();

    g_transform.m_fRotateNum += 0.0002f;

    g_World = XMMatrixScaling(g_transform.m_v3Scale.x,
        g_transform.m_v3Scale.y,
        g_transform.m_v3Scale.z) *

        XMMatrixRotationX(g_transform.m_v3Rotate.x) *
        XMMatrixRotationY(g_transform.m_v3Rotate.y) *
        XMMatrixRotationZ(g_transform.m_v3Rotate.z) *

        XMMatrixTranslation(g_transform.m_v3Position.x,
            g_transform.m_v3Position.y,
            g_transform.m_v3Position.z);


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
    g_World = XMMatrixScaling(1, 1, 1) *
        XMMatrixRotationRollPitchYaw(0, t, 0) *
        XMMatrixTranslation(0, 0, 0);

    // Modify the color
    g_vMeshColor.x = 1.0f;//( sinf( t * 1.0f ) + 1.0f ) * 0.5f;
    g_vMeshColor.y = 1.0f;//( cosf( t * 3.0f ) + 1.0f ) * 0.5f;
    g_vMeshColor.z = 1.0f;//( sinf( t * 5.0f ) + 1.0f ) * 0.5f;

    // Update variables that change once per frame
    CBChangesEveryFrame cb;
    cb.mWorld = g_World;
    cb.vMeshColor = g_vMeshColor;
    g_modelBuffer.update(g_deviceContext, 0, nullptr, &cb, 0, 0);

    // Update light
    //g_lightConfig.LightPos = XMFLOAT4(5.0f, 6.0f, -5.0f, 0); // Posición de la luz en el espacio 3D
    g_lightConfig.LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f); // Color de la luz en RGB
    g_lightConfig.AmbientIntensity = 0.05f; // Intensidad ambiental de la luz+
    g_lightConfig.padding = 0.0f; // Padding to align to 16 bytes
    g_lightBuffer.update(g_deviceContext, 0, nullptr, &g_lightConfig, 0, 0);

    // Update Camera Buffers
    g_camera.update(g_deviceContext, 0, nullptr, &g_cameraData, 0, 0);
}


void Render()
{
    // Clear the back buffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_depthStencilView.render(g_deviceContext);
    g_renderTargetView.render(g_deviceContext, g_depthStencilView, 1, ClearColor);
    // Set viewport
    g_viewport.render(g_deviceContext);

    // Set camera buffers
    g_shaderProgram.render(g_deviceContext);

    // Set constant buffer for changes every frame
    g_camera.renderModel(g_deviceContext, 0, 1);
    g_modelBuffer.renderModel(g_deviceContext, 1, 1);
    g_lightBuffer.renderModel(g_deviceContext, 2, 1);

    // Set sampler, vertex buffer, index buffer, and texture
    g_sampler.render(g_deviceContext, 0, 1);
    g_vertexBuffer.render(g_deviceContext, 0, 1);
    g_indexBuffer.render(g_deviceContext, DXGI_FORMAT_R32_UINT);
    g_albedo.render(g_deviceContext, 0, 1);
    g_normal.render(g_deviceContext, 1, 1);
    g_sampler.render(g_deviceContext, 1, 1);

    // Set primitive topology and draw the indexed mesh
    g_deviceContext.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_deviceContext.m_deviceContext->DrawIndexed(g_mesh.numIndex, 0, 0);

    //Ui
    UI.render();


    // Present our back buffer to our front buffer
    g_swapchain.present();
}