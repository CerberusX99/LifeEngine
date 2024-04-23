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
#include "ShaderProgram.h"
#include "SwapChain.h"
#include "RenderTargetView.h"
#include "Viewport.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "ModelLoader.h"
#include "UserInterface.h"


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
Buffer                              g_camera;
Buffer                              g_modelBuffer;
Buffer                              g_lightBuffer;
Texture                             g_albedo;
Texture                             g_normal;
SamplerState                        g_sampler;
UserInterface                       UI;


XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
Mesh                                g_mesh;
ModelLoader                         g_modelLoader;
Camera                              g_cameraData;
LightConfig                         g_lightConfig;
float                               g_LightMovementSpeed = 1.0;
XMFLOAT4                            g_LightPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
float                               scaleX = 1.0f;
float                               scaleY = 1.0f;
float                               scaleZ = 1.0f;
float                               rotationAngleX = 0.0f;
float                               rotationAngleY = 0.0f;
float                               rotationAngleZ = 0.0f;
float                               translationX = 0.0f;
float                               translationY = 0.0f;
float                               translationZ = 0.0f;
float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
bool                                rotateObject=false;




//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void update();
void Render();
/*void UpdateLightPosition(const XMFLOAT4& newPosition)
{
    g_lightConfig.LightPos = newPosition,

    g_lightBuffer.update(g_deviceContext, 0, nullptr, &g_lightConfig, 0, 0);
}*/


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
    normal.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT /*12*/;
    normal.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    normal.InstanceDataStepRate = 0;
    Layout.push_back(normal);

    //init Shader
    g_shaderProgram.init(g_device, "ShaderV2.fx", Layout);
    //g_shaderProgram.init(g_device, "BananasEngine.fx", Layout);

    //Generar Mesh
    g_mesh = g_modelLoader.Load("mona.obj");
    //->Carga de modelo 3D
    //Create vertex buffer
    g_vertexBuffer.init(g_device, g_mesh, D3D11_BIND_VERTEX_BUFFER);

    g_indexBuffer.init(g_device, g_mesh, D3D11_BIND_INDEX_BUFFER);

    g_camera.init(g_device, sizeof(Camera));

    g_modelBuffer.init(g_device, sizeof(CBChangesEveryFrame));
    g_lightBuffer.init(g_device, sizeof(LightConfig));

    //Load the texture 
    g_albedo.init(g_device, "monaal.png");
   g_normal.init(g_device, "monanor.png");

    //Create the sampler state
    g_sampler.init(g_device);

    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, 6.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    g_View = XMMatrixLookAtLH(Eye, At, Up);

    g_cameraData.mView = XMMatrixTranspose(g_View);
    // Initialize the projection matrix
    g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_window.m_width / (float)g_window.m_height, 0.01f, 100.0f);

    g_cameraData.mProjection = XMMatrixTranspose(g_Projection);

    g_lightConfig.LightPos = XMFLOAT4(0.0f, 0.0f, 70.0f, 0); //Posicion de la luz en el espacio 3D
    g_lightConfig.LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f); //Color de luz RGB
    g_lightConfig.AmbientIntensity = 0.2f; //Intensidad Ambiental de la luz

    UI.init(g_window.m_hWnd, g_device.m_device, g_deviceContext.m_deviceContext);

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
    g_albedo.destroy();
    g_normal.destroy();
    //Releas Camera resources
    g_camera.destroy();
    //Release Model resources
    g_modelBuffer.destroy();
    g_lightBuffer.destroy();
    // Release Shader Resources
    g_vertexBuffer.destroy();
    g_vertexBuffer.destroy();

    //Release depth stencil

    g_shaderProgram.destroy();
    g_depthStencil.destroy();
    //Release depth stencil view
    g_depthStencilView.destroy();
    //Release render target view
    g_renderTargetView.destroy();
    //Release swapchain
    g_swapchain.destroy();
    //Release UI
    // 
    //Release device 
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
    bool showDemoWindow = true;
    ImGui::ShowDemoWindow(&showDemoWindow);

    //Light Settings
    ImGui::Begin("Light Settings");

    ImGui::SliderFloat("Light X", &g_lightConfig.LightPos.x, -100.0f, 100.0f);
    ImGui::SliderFloat("Light Y", &g_lightConfig.LightPos.y, -100.0f, 100.0f);
    ImGui::SliderFloat("Light Z", &g_lightConfig.LightPos.z, -100.0f, 100.0f);
    ImGui::SliderFloat("Light Intensity", &g_lightConfig.LightIntensity, 0.0f, 1.0f);
    ImGui::ColorEdit3("Light Color", (float*)&g_lightConfig.LightColor);
    ImGui::SliderFloat("Ambient Intensity", &g_lightConfig.AmbientIntensity, 0.0f, 1.0f);

    ImGui::End();

    //MeshColor Settings
    ImGui::Begin("vMeshColor");

    ImGui::SliderFloat("vMeshColorX", &g_vMeshColor.x, -100.0f, 100.0f);
    ImGui::SliderFloat("vMeshColorY", &g_vMeshColor.y, -100.0f, 100.0f);
    ImGui::SliderFloat("vMeshColorZ", &g_vMeshColor.z, -100.0f, 100.0f);
    ImGui::SliderFloat("vMeshColorW", &g_vMeshColor.w, -100.0f, 100.0f);

    ImGui::End();

    //Transform model settings
    ImGui::Begin("Object Transform");

    // Scale
    ImGui::Text("Scale:");
    ImGui::SliderFloat("Scale X", &scaleX, 0.1f, 10.0f);
    ImGui::SliderFloat("Scale Y", &scaleY, 0.1f, 10.0f);
    ImGui::SliderFloat("Scale Z", &scaleZ, 0.1f, 10.0f);

    // Rotación
    ImGui::Text("Rotate:");
    ImGui::SliderFloat("Ángle X", &rotationAngleX, 0.0f, 90.0f);
    ImGui::SliderFloat("Ángle Y", &rotationAngleY, 0.0f, 90.0f);
    ImGui::SliderFloat("Ángle Z", &rotationAngleZ, 0.0f, 90.0f);
    ImGui::Checkbox("Rotate Object", &rotateObject);

    // Translación
    ImGui::Text("Translate:");
    ImGui::SliderFloat("Translate X", &translationX, -10.0f, 10.0f);
    ImGui::SliderFloat("Translate Y", &translationY, -10.0f, 10.0f);
    ImGui::SliderFloat("Translate Z", &translationZ, -10.0f, 10.0f);


    ImGui::End();
    //BackGroundd Settings
    ImGui::Begin("BackGround Color");

    ImGui::ColorEdit4("BackGround", (float*)&ClearColor);

    ImGui::End();
    //Camera Settings
   


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    // Update time
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

    // Determina angulo de rotacion y velocidad
    if (rotateObject) {
        const float rotationSpeed = XM_PI / 1100.1f; // Adjust rotation speed as needed
       // rotationAngleX += rotationSpeed * t;
        rotationAngleY += rotationSpeed;
        //rotationAngleZ += rotationSpeed * t;
    }

    // Apply transformations
    g_World = XMMatrixScaling(scaleX, scaleY, scaleZ) *
        XMMatrixRotationRollPitchYaw(rotationAngleX, rotationAngleY, rotationAngleZ) *
        XMMatrixTranslation(translationX, translationY, translationZ);

    // Update variables that change once per frame
    CBChangesEveryFrame cb;
    cb.mWorld = XMMatrixTranspose(g_World);
    cb.vMeshColor = g_vMeshColor;
    g_modelBuffer.update(g_deviceContext, 0, nullptr, &cb, 0, 0);

    g_lightBuffer.update(g_deviceContext, 0, nullptr, &g_lightConfig, 0, 0);

    // Update Camera Buffers
    g_camera.update(g_deviceContext, 0, nullptr, &g_cameraData, 0, 0);
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------

void Render()
{
    // Clear the back buffer
    //float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
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