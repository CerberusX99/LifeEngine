#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

class Buffer
{
public:
	Buffer() = default;
	~Buffer() {};

	//Inicializa Vertex e Index Buffers
	void
		init(Device device, Mesh mesh, unsigned int bindFlag);

	//Inicializa Constant Buffers
	void
		init(Device device, unsigned int ByteWidth);

	//Actualizamos la logica del Constant Buffer
	void
		update(DeviceContext& deviceContext,
			unsigned int DstSubresource,
			const D3D11_BOX* pDstBox,
			const void* pSrcData,
			unsigned int SrcRowPitch,
			unsigned int SrcDepthPitch);

	//Actualiza en render el Vertex e IndexBuffer
	void
		render(DeviceContext& deviceContext, unsigned int StarSlot, unsigned int NumBuffers);

	//Actualiza en render el constsnt Buffer

	void
		render(DeviceContext& deviceContext, DXGI_FORMAT format);


	void
		destroy();

private:
	ID3D11Buffer* m_buffer = nullptr;
	unsigned int m_stride = 0;
	unsigned int m_offset = 0;
	unsigned int m_bindFlag = 0;
};
