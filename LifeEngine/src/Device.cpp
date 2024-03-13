#include "Device.h"

void Device::init()
{
}

void Device::update()
{
}

void Device::render()
{
}

void Device::destroy()
{
	SAFE_RELEASE(m_device);
}

// Este metodo se utiliza para crear una vista de destino de renderizacion
// (render target view) a partir de un recurso de DirectX 11.
//Toma como parametros el recurso, una descripcion de la vista de destino de 
// renderizacion y un puntero al objeto ID3D11RenderTargetView donde se almacenara.
HRESULT Device::CreateRenderTargetView(ID3D11Resource* pResource,
	const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
	ID3D11RenderTargetView** ppRTView)
{
	HRESULT hr = S_OK;
	//Check if the resource and the RTView exist
	if (pResource != nullptr || ppRTView != nullptr)
	{
		hr = m_device->CreateRenderTargetView(pResource, pDesc, ppRTView);
		WARNING("Device::CreateRenderTargetView : [CREATION OF RESOURCE : OK ] \n");
	}
	else
	{
		WARNING("ERROR: DEVICE:: CreateRenderTargetView : Error in data from params [CHECK FOR CREATION METHOD] \n");
		exit(1);
	}
	return hr;
}


//El metodo es para crear una textura 2D de DirectX11. 
//Toma como Parametros una descripcion de la textura D3D11_TEXTURE2D_DESC, 
// datos iniciales, un puntero al objeto ID3D11Texture2D donde se almacenara 
//la textura creada
HRESULT __stdcall Device::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
	const D3D11_SUBRESOURCE_DATA* pInitialData,
	ID3D11Texture2D** ppTexture2D)
{
	HRESULT hr = S_OK;
	//Che
	if (pDesc != nullptr || ppTexture2D != nullptr)
	{
		hr != m_device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
		WARNING("Device::CreateTexture2D : [CREATION OF RESOURCE : OK ] \n");

	}
	else
	{
		WARNING("ERROR: Device::CreateTexture2D : Error in data from params [CHECK FOR CREATION METHOD] \n");
		exit(1);
	}
	return hr;
}

//Crea una vista en el stencil a partir de un recurso de DirecX11.
HRESULT Device::CreateDepthStencilView(ID3D11Resource* pResource,
	const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
	ID3D11DepthStencilView** ppDepthStencilView)
{
	HRESULT hr = S_OK;
	if (pResource == nullptr)
	{
		WARNING("ERROR: Device::CreateDepthStencilView Error in data from params [CHECK FOR CREATION ID3D11Resorce* pResource] \n");
		exit(1);
	}
	else if (ppDepthStencilView == nullptr)
	{
		WARNING("ERROR: Device::CreateDepthStencilView Error in data from params [CHECK FOR CREATION ID3D11DepthStencilView** ppDepthStencilView] \n");
		exit(1);
	}
	else
	{
		hr = m_device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
		WARNING("Device::CreateDepthStencilView : [CREATION OF RESOURCE : OK] \n");
	}
	return hr;
}

//Se utiliza para crear un sombreador de Vertices en DirectX11.
//Toma parametros del codigo de bytes del sombreador, la longitud del codigo
// de bytes y se almacenan
HRESULT Device::CreateVertexShader(const void* pShaderBytecode,
	unsigned int BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage,
	ID3D11VertexShader** ppVertexShader)
{
	HRESULT hr = S_OK;

	if (pShaderBytecode == nullptr)
	{
		WARNING("ERROR: Device::CreateVertexShader : Error in data from params [CHECK FOR const void* pShaderBytecode] \n");
		exit(1);
	}
	else if (ppVertexShader == nullptr)
	{
		WARNING("ERROR: Device::CreateVertexShader : Error in data from params [CHECK FOR ID3D11VertexShader** ppVertexShader] \n");
		exit(1);
	}
	else
	{
		hr = m_device->CreateVertexShader(pShaderBytecode,
			BytecodeLength,
			pClassLinkage,
			ppVertexShader);
		WARNING("Device::CreateVertexShader : [CREATION OF RESOURCE : OK] \n");
	}

	return hr;
}

//Crea un dise;o de entrada en el DirectX11. 
HRESULT Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
	unsigned int NumElements,
	const void* pShaderBytecodeWithInputSignature,
	unsigned int BytecodeLength,
	ID3D11InputLayout** ppInputLayout)
{
	HRESULT hr = S_OK;
	if (pInputElementDescs == nullptr)
	{
		WARNING("ERROR: Device::CreateInputLayout : Error in data from params [CHECK FOR CONST D3D11_INPUT_ELEMENT_DESC* pInputDescs]\n");
		exit(1);
	}
	else if (pShaderBytecodeWithInputSignature == nullptr)
	{
		WARNING("ERROR: Device::CreateInputLayout : Error in data from params [CHECK FOR const void* pShaderBytecodeWithInputSignature]\n");
		exit(1);
	}
	else if (ppInputLayout == nullptr)
	{
		WARNING("ERROR: Device::CreateInputLayout : Error in data from params [CHECK FOR ID3D11InputLayout** ppInputLayout]\n");
		exit(1);
	}
	else
	{
		hr = m_device->CreateInputLayout(pInputElementDescs,
			NumElements,
			pShaderBytecodeWithInputSignature,
			BytecodeLength,
			ppInputLayout);
		WARNING("Device::CreateInputLayout : [CREATION OF RESOURCE : OK ] \n");
	}
	return hr;
}

//El metodo se usa para sombrear los pixeles 
//Toma como parametros el codigo de bytes del sombreador, la longitud del codigo
//de bytes, un objeto ID3D11ClassLinkage y puntero donde se almacenan
HRESULT Device::CreatePixelShader(const void* pShaderBytecode,
	unsigned int BytecodeLength,
	ID3D11ClassLinkage* pClassLinkage,
	ID3D11PixelShader** ppPixelShader)
{
	HRESULT hr = S_OK;

	if (pShaderBytecode == nullptr)
	{
		WARNING("ERROR: Device::CreatePixelShader : Error in data from params [CHECK FOR const void* pShaderBytecode] \n");
		exit(1);
	}
	else if (ppPixelShader == nullptr)
	{
		WARNING("ERROR: Device::CreatePixelShader : Error in data from params [CHECK FOR ID3D11PixelShader** ppPixelShader] \n");
		exit(1);
	}
	else
	{
		hr = m_device->CreatePixelShader(pShaderBytecode,
			BytecodeLength,
			pClassLinkage,
			ppPixelShader);
		WARNING("Device::CreatePixelShader : [CREATION OF RESOURCE : OK] \n");
	}
	return hr;
}

//El metodo crea un buffer
HRESULT Device::CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
	const D3D11_SUBRESOURCE_DATA*
	pInitialData, ID3D11Buffer** ppBuffer)
{
	HRESULT hr = S_OK;

	if (pDesc == nullptr)
	{
		WARNING("ERROR: Device::CreateBuffer : Error in data from params [CHECK FOR const D3D11_BUFFER_DESC* pDesc] \n");
		exit(1);
	}
	else if (ppBuffer == nullptr)
	{
		WARNING("ERROR: Device::CreateBuffer : Error in data from params [CHECK FOR ID3D11Buffer** ppBuffer ] \n");
		exit(1);
	}
	else
	{
		hr = m_device->CreateBuffer(pDesc, pInitialData, ppBuffer);
		WARNING("Device::CreateBuffer : [CREATION OF RESOURCE : OK] \n");
	}

	return hr;
}

//Este metodo se utiliza para crear un estado de 
// muestreador (sampler state) en DirectX 11.
HRESULT Device::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
	ID3D11SamplerState** ppSamplerState)
{
	HRESULT hr = S_OK;

	if (pSamplerDesc == nullptr)
	{
		WARNING("ERROR: Device::CreateSamplerState : Error in data from params [CHECK FOR const D3D11_SAMPLE_DESC* pSamplerDesc] \n");
		exit(1);
	}
	else if (ppSamplerState == nullptr)
	{
		WARNING("ERROR: Device::CreateSamplerState : Error in data from params [CHECK FOR const ID3D11SamplerState** ppSamplerState] \n");
		exit(1);
	}
	else
	{
		hr = m_device->CreateSamplerState(pSamplerDesc, ppSamplerState);
		WARNING("Device::CreateSamplerState : [CREATION OF RESOURCE : OK] \n");
	}

	return hr;
}
