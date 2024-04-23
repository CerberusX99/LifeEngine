#pragma once
#include "Prerequisites.h"
class Device;
class DeviceContext;

/**
  * brief: Esta clase textura esta encargada de almacenar proporcionar,
  * recursos de texturas (Texels).
  */
class
	Texture {
public:
	Texture() = default;
	~Texture();

	/// <summary>
	/// Brief: Crea una textura a partir de una imagen (.dds) desde nuestro ordenador.
	/// </summary>
	/// <param name="device">: Llamamos al device para poder generar los recursos en memoria. </param>
	/// <param name="textureName">: Almacenamos el nombre de la textura, para cargala desde memoria. </param>
	void
		init(Device device, std::string textureName);

	
	void
		init(Device device,
			unsigned int width,
			unsigned int height,
			DXGI_FORMAT Format,
			unsigned int BindFlags);

	void
		update();

	
	void
		render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumViews);

	
	void
		destroy();

public:
	// This variable is in charge of handle a texture resource as data
	ID3D11Texture2D* m_texture = nullptr;
	// This variable is in charge of handle a texture resource as image data
	ID3D11ShaderResourceView* m_textureFromImg;
};