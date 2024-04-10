//--------------------------------------------------------------------------------------
// File: JetrayEngine.cpp
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
// Global Variables
//--------------------------------------------------------------------------------------
Window                              g_window;
Device															g_device;
DeviceContext												g_deviceContext;
Texture															g_depthStencil;
DepthStencilView										g_depthStencilView;
SwapChain														g_swapchain;
Texture															g_backBuffer;
RenderTargetView                    g_renderTargetView;
Viewport														g_viewport;
ShaderProgram												g_shaderProgram;
Buffer															g_vertexBuffer;
Buffer															g_indexBuffer;
Buffer															g_camera;
Buffer															g_modelBuffer;
Buffer															g_lightBuffer;
Texture															g_albedo;
Texture															g_normal;
SamplerState                        g_sampler;

XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
Mesh																g_mesh;
ModelLoader													g_loader;
Camera                              g_cameraData;
LightConfig													g_lightConfig;
float															  g_LightMovementSpeed = 1.0f;
XMFLOAT4													  g_LightPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void update();
void Render();
void UpdateLightPosition(const XMFLOAT4& newPosition)
{
	// Actualiza la posición de la luz en tu estructura LightBuffer
	g_lightConfig.LightPos = newPosition;

	// Actualiza el buffer constante en el dispositivo con la nueva información
	g_lightBuffer.update(g_deviceContext, 0, nullptr, &g_lightConfig, 0, 0);
}

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
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	g_swapchain.init(g_device, g_deviceContext, g_backBuffer, g_window);

	// Create render target view
	g_renderTargetView.init(g_device, g_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);

	// Create depth stencil texture
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

	// Define the input layout

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

	// Init shader
	g_shaderProgram.init(g_device, "ShaderV2.fx", Layout);

	// Generar Mesh
	g_mesh = g_loader.Load("Pistol.obj");

	// Create vertex buffer
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

	g_lightConfig.LightPos = XMFLOAT4(0.0f, 0.0f, 80.0f, 0); // Posición de la luz en el espacio 3D
	g_lightConfig.LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f); // Color de la luz en RGB
	g_lightConfig.AmbientIntensity = 0.5f; // Intensidad ambiental de la luz

	return S_OK;
}


// Clean up the objects we've created
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


// Called every time the application receives a message
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
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT: // Flecha izquierda
			g_LightPosition.x -= g_LightMovementSpeed;
			break;
		case VK_RIGHT: // Flecha derecha
			g_LightPosition.x += g_LightMovementSpeed;
			break;
		case VK_UP: // Flecha arriba
			g_LightPosition.y += g_LightMovementSpeed;
			break;
		case VK_DOWN: // Flecha abajo
			g_LightPosition.y -= g_LightMovementSpeed;
			break;
		case 'W': // Tecla W para mover hacia adelante en el eje Z
			g_LightPosition.z += g_LightMovementSpeed;
			break;
		case 'S': // Tecla S para mover hacia atrás en el eje Z
			g_LightPosition.z -= g_LightMovementSpeed;
			break;
		}

		// Aquí puedes llamar a una función para actualizar la posición de la luz en tu shader
		//UpdateLightPosition(g_LightPosition);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// Update every frame
void update() {
	// Update our time
	static float t = 0.0f;
	if (g_swapchain.m_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static unsigned int dwTimeStart = 0;
		unsigned int dwTimeCur = GetTickCount();
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
	g_lightConfig.LightColor = XMFLOAT3(1.0f, 50.0f, 1.0f); // Color de la luz en RGB
	g_lightConfig.AmbientIntensity = 0.01f; // Intensidad ambiental de la luz+
	g_lightConfig.padding = 0.0f; // Padding to align to 16 bytes
	g_lightBuffer.update(g_deviceContext, 0, nullptr, &g_lightConfig, 0, 0);

	// Update Camera Buffers
	g_camera.update(g_deviceContext, 0, nullptr, &g_cameraData, 0, 0);
}


// Render a frame
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

	// Present our back buffer to our front buffer
	g_swapchain.present();
}

