#pragma once
#include "Prerequisites.h"
class Device;
class DeviceContext;



class
Texture {
public:
	Texture() = default;
	~Texture();


	
	void
	init(Device device, std::string textureName);

	

	void 
	init(Device device,
		unsigned int width,
		unsigned int height,
		DXGI_FORMAT Format,
		unsigned int BindFlags);

	//Cambios dinamicos en la textura 
	void
		update();


	void
		render(DeviceContext & deviceContext, unsigned int StartSlot);

	void
		destroy();

public:

	ID3D11Texture2D* m_texture = nullptr;
	ID3D11ShaderResourceView* m_textureFromImg = nullptr;
};