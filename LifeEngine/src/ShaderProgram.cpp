#include "ShaderProgram.h"
#include "Device.h"
#include "DeviceContext.h"

void ShaderProgram::init(Device device,
	std::string fileName,
	std::vector<D3D11_INPUT_ELEMENT_DESC> Layout)
{
	if (device.m_device == nullptr)
	{
		WARNING("ERROR: ShaderProgram::init : Error in data from params [CHECK FOR Device device]\n");
		exit(1);
	}
	else if (Layout.size() <= 1)
	{
		WARNING("ERROR: ShaderProgram::init : Error in data from params [CHECK FOR Device std::vector<D3D11_INPUT_ELEMENT_DESC> Layout]\n");
		exit(1);
	}
	//Store the shade file name
	m_shaderFileName = fileName;
	//Create the vertex shader
	CreateVertexShader(device);
	//Create input layout
	CreateInputLayout(device, Layout);
	//Create the pixel Shader
	CreatePixelShader(device);
}

void ShaderProgram::update()
{
}

void ShaderProgram::render(DeviceContext& deviceContext)
{
	//Establecer el Input Layout
	m_inputLayout.render(deviceContext);
	//Establecer shaders
	deviceContext.VSSetShader(m_VertexShader, nullptr, 0);
	deviceContext.PSSetShader(m_PixelShader, nullptr, 0);

}

void ShaderProgram::destroy()
{
	SAFE_RELEASE(m_VertexShader);
	m_inputLayout.destroy();
	SAFE_RELEASE(m_PixelShader);
}



HRESULT ShaderProgram::CompileShaderFromFile(char* szFileName,
	LPCSTR szEntryPoint,
	LPCSTR szShaderModel,
	ID3DBlob** ppBlobOut)
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

void ShaderProgram::CreateVertexShader(Device device)
{
	HRESULT hr = S_OK;
	//Compile the vertex Shader
	hr = CompileShaderFromFile(m_shaderFileName.data(), "VS", "vs_4_0", &m_vertexShaderData);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		WARNING("ERROR: ShaderProgram::CreateVertexShader : Error in data from params [CHECK FOR CompileShaderFromFile()]\n");
		exit(1);
	}
	//Create the vertex shader
	hr = device.CreateVertexShader(m_vertexShaderData->GetBufferPointer(),
		                           m_vertexShaderData->GetBufferSize(),
		                           nullptr,
		                           &m_VertexShader);
	if (FAILED(hr))
	{
		m_vertexShaderData->Release();
		WARNING("ERROR: ShaderProgram::CreateVertexShader : Error in data from params [CHECK FOR device.CreateVertexShader()]\n");
		exit(1);
	}
}

void ShaderProgram::CreateInputLayout(Device device,
	std::vector<D3D11_INPUT_ELEMENT_DESC> Layout)
{
	m_inputLayout.init(device, Layout, m_vertexShaderData);
	m_vertexShaderData->Release();
}

void ShaderProgram::CreatePixelShader(Device device)
{
	HRESULT hr = S_OK;
	//Compile the vertex Shader
	hr = CompileShaderFromFile(m_shaderFileName.data(), "PS", "ps_4_0", &m_pixelShaderData);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		WARNING("ERROR: ShaderProgram::CreatePixelShader : Error in data from params [CHECK FOR CompileShaderFromFile()]\n");
		exit(1);
	}
	//Create the vertex shader
	hr = device.CreatePixelShader(m_pixelShaderData->GetBufferPointer(),
		m_pixelShaderData->GetBufferSize(),
		nullptr,
		&m_PixelShader);
	if (FAILED(hr))
	{
		m_pixelShaderData->Release();
		WARNING("ERROR: ShaderProgram::CreatePixelShader : Error in data from params [CHECK FOR device.CreatePixelShader()]\n");
		exit(1);
	}
}