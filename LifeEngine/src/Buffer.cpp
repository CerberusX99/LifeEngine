#include "Buffer.h"
#include "Device.h"
#include "DeviceContext.h"

void Buffer::init(Device device, Mesh mesh, unsigned int bindFlag)
{
	//Revisamos, si device es nulo
	if (device.m_device == nullptr)
	{
		WARNING("ERROR: Buffer::init : Error in data from params [CHECK FOR Device device]\n");
		exit(1);

	}

	//Definimos el descriptor de la estructura vertex o index
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA InitData;

	memset(&desc, 0, sizeof(desc));
	memset(&InitData, 0, sizeof(InitData));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	m_bindFlag = bindFlag;
	//Validamos el tipo de buffer a ser utilizado
	if (bindFlag == D3D11_BIND_VERTEX_BUFFER)
	{
		if (mesh.vertex.size() == 0 || mesh.numVertex == 0)
		{
			WARNING("ERROR: Buffer::init : Error in data from params [CHECK FOR Mesh mesh VERTEX]\n");
			exit(1);
		}
		//Init descriptor
		m_stride = sizeof(SimpleVertex);
		desc.ByteWidth = m_stride * mesh.numVertex;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//Init Subresource
		InitData.pSysMem = mesh.vertex.data();
	}
	if (bindFlag == D3D11_BIND_INDEX_BUFFER)
	{
		if (mesh.index.size() == 0 || mesh.numIndex == 0)
		{
			WARNING("ERROR: Buffer::init : Error in data from params [CHECK FOR Mesh mesh INDEX]\n");
			exit(1);
		}
		//Init descriptor
		m_stride = sizeof(unsigned int);
		desc.ByteWidth = m_stride * mesh.numIndex;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//Init Subresource
		InitData.pSysMem = mesh.index.data();
	}

	//Creamos el Buffer

	hr = device.CreateBuffer(&desc, &InitData, &m_buffer);

	if (FAILED(hr))
	{
		WARNING("ERROR: Buffer::init : Error in data from method [CHECK FOR CreateBuffer()]\n");
		exit(1);
	}
}

void Buffer::init(Device device, unsigned int ByteWidth)
{
	if (device.m_device == nullptr)
	{
		WARNING("ERROR: Buffer::init : Error in data from params [CHECK FOR Device device]\n");
		exit(1);
	}

	else if (ByteWidth == 0)
	{
		WARNING("ERROR: Buffer::init : Error in data from params [CHECK FOR unsigned int ByteWidth]\n");
		exit(1);
	}

	m_stride = ByteWidth;

	//Definimmos el descriptor de la estructura vertex o index
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.ByteWidth = m_stride;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	//Creamos el buffer
	hr = device.CreateBuffer(&desc, nullptr, &m_buffer);
	if (FAILED(hr))
	{
		WARNING("ERROR: Buffer::init : Error in data from method [CHECK FOR CreateBuffer()] \n");
		exit(1);
	}
}

void Buffer::update(DeviceContext& deviceContext, 
	                unsigned int DstSubresource, 
	                const D3D11_BOX* pDstBox, 
	                const void* pSrcData, 
	                unsigned int SrcRowPitch, 
	                unsigned int SrcDepthPitch)
{
	deviceContext.UpdateSubresource(m_buffer,
		                            DstSubresource,
		                            pDstBox,
		                            pSrcData,
		                            SrcRowPitch,
		                            SrcDepthPitch);
}

void Buffer::render(DeviceContext& deviceContext,
	                unsigned int StarSlot,
	                unsigned int NumBuffers)
{
	if (m_bindFlag == D3D11_BIND_VERTEX_BUFFER)
	{
		deviceContext.IASetVertexBuffer(StarSlot, NumBuffers,&m_buffer ,&m_stride , &m_offset);
	}
	else
	{
		WARNING("WARNING: Buffer::render : Trying to render in D3D11_IND_INDEX_BUFFER BidFlag, Check for methods render()\n");

	}
}

void Buffer::render(DeviceContext& deviceContext, DXGI_FORMAT format)
{
	if (m_bindFlag == D3D11_BIND_INDEX_BUFFER)
	{
		deviceContext.IASetIndexBuffer(m_buffer, format, m_offset);
	}
	else
	{
		WARNING("WARNING: Buffer::render : Trying to render in D3D11_IND_VERTEX_BUFFER BidFlag, Check for methods render()\n");

	}
}

void Buffer::destroy()
{
	SAFE_RELEASE(m_buffer);
}