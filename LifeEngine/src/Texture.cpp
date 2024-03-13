#include "Texture.h"
#include "Device.h"
#include "DeviceContext.h"

Texture::~Texture()
{
	/*
	if (m_texture != nullptr)
	{
		SAFE_RELEASE(m_texture);
	}
	else if (m_textureFromImg != nullptr)
	{
		SAFE_RELEASE(m_textureFromImg);
	}
	*/
}

//Texture::init(Device device, std::string textureName)
//Inicializa la textura desde un archivo de imagen. Crea una vista de recurso
//  (ID3D11ShaderResourceView) a partir de un archivo de imagen 
// especificado (textureName).
//Tambien se asegura de que el device no sea nulo y maneja los errores.
void Texture::init(Device device, std::string textureName)
{

	if (device.m_device == nullptr)
	{
		WARNING("ERROR: Texture::init : Error in data from params [CHECK FOR Device device] \n");
		exit(1);
	}

	HRESULT hr = S_OK;

	hr = D3DX11CreateShaderResourceViewFromFile(device.m_device,
		                                        textureName.c_str(),
		                                        nullptr, 
		                                        nullptr, 
		                                        &m_textureFromImg,
		                                        nullptr);

	if (FAILED(hr)) {
		WARNING("ERROR: Texture::init : Error in data from params [CHECK FOR string textureName -> Verify correct Texture name in Filepath] \n ");
		exit(1);
	}
	

}

//Inicializa la textura con parametros especificos como ancho, alto, formato, etc. 
//Se crea una textura 2D
//Maneja los errores
void Texture::init(Device device, 
	               unsigned int width, 
	               unsigned int height, 
	               DXGI_FORMAT Format, 
	               unsigned int BindFlags)
{
	if (device.m_device == nullptr)
	{
		WARNING("ERROR: Texture::init : Error in data from params [CHECK FOR Device device] \n");
		exit(1);
	}
	else if (width <= 0 || height <= 0) 
	{
		WARNING("ERROR: Texture::init : Error in data from params {CHECK FOR unsigned int width OR unsigned int height] \n");
		exit(1);
	}
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = Format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = BindFlags;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	//Create a depth stencil texture

	if (BindFlags == D3D11_BIND_DEPTH_STENCIL) {
		hr = device.CreateTexture2D(&desc, nullptr, &m_texture);
	}
	else if (BindFlags == D3D11_BIND_RENDER_TARGET) {
		hr = device.CreateTexture2D(&desc, nullptr, &m_texture);
	}

	if (m_texture == nullptr)
	{
		WARNING("ERROR: Testure::init : Error in data from params [CHECK FOR m_texture ] \n");
		exit(1);
	}
	else if (FAILED(hr)) {
		WARNING("ERROR: Texture::init : Error in data from resource [CHECK FOR CreateTexture2D] \n");
		exit(1);
	}
}

//Renderiza la textura su fue inicializada desde un archivo imagen.
void Texture::render(DeviceContext& deviceContext, unsigned int StartSlot)
{
	if (m_textureFromImg != nullptr)
	{
		ID3D11ShaderResourceView* nullSRV[] = { nullptr };
		deviceContext.PSSetShaderResources(StartSlot, 1, nullSRV);
		deviceContext.PSSetShaderResources(StartSlot, 1, &m_textureFromImg);
	}
}

//Destruye la textura liberando la memoria asociada.
void Texture::destroy()
{
	if (m_texture != nullptr)
	{
		SAFE_RELEASE(m_texture);
	}
	else if (m_textureFromImg != nullptr)
	{
		SAFE_RELEASE(m_textureFromImg);
	}
}
