#include "DepthStencilView.h"
#include "Device.h"
#include "DeviceContext.h"

//El metodo  toma como parametros el device, un recurso el 
//stencil (ID3D11Resource).
//Se verifican que el dispositivo y el stencil no sean nulos
void
DepthStencilView::init(Device device, ID3D11Resource* depthStencil, DXGI_FORMAT format)
{
	if (device.m_device == nullptr)
	{

		WARNING("ERROR: Device::DepthStencilView::init Error in data from params [ CHECK FOR Device device] \n");
		exit(1);
	}
	else if (depthStencil == nullptr)
	{
		WARNING("ERROR: Device::DepthStencilView::init Error in data from params [ CHECK FOR ID3D11Resource* depthStecil] \n");
		exit(1);
	}
	else
	{
		//Configurar la descripcion del stencilview
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		memset(&descDSV,0, sizeof(descDSV));
		descDSV.Format = format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		
		// Crear la vista 
		device.CreateDepthStencilView(depthStencil, &descDSV, &m_depthStencilView);
	}
}

// Se encarga de realizar el borrado del stencil utilizando DeviceContext y
// lo toma como parametro
//Utiliza el metodo ClearDepthStencilView() para limpiar el stencil, en este caso,
// se esta borrando solo la parte de D3D11_CLEAR_DEPTH y se establece un valor de 1.0f
void DepthStencilView::render(DeviceContext& deviceContext)
{
	deviceContext.ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

// Método para liberar la memoria asignada para la vista de profundidad y stencil
void DepthStencilView::destroy()
{
	// Liberar la memoria de la vista de profundidad y stencil
	SAFE_RELEASE(m_depthStencilView);
}
