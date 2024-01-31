#include "DepthStencilView.h"
#include "Device.h"
#include "DeviceContext.h"
void
DepthStencilView::init(Device device, ID3D11Resource* depthStencil, DXGI_FORMAT format)
{
	if (device.m_device == nullptr)
	{
		WARNING("ERROR: Device::DepthStencilView::init : Error in data from params [CHECK FOR Device device]  \n ");
		exit(1);
	}
	else if (depthStencil == nullptr)
	{
		WARNING("ERROR: Device::DepthStencilView::init : Error in data from params [CHECK FOR ID3D11Resource*depthStencil]  \n ");
		exit(1);
	}
	else {
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		memset (&descDSV, 0, sizeof(descDSV));
		descDSV.Format = format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		device.CreateDepthStencilView(depthStencil, &descDSV, &m_depthStencilView);
	}

}

void DepthStencilView::update()
{
}
void 
DepthStencilView::render(DeviceContext& deviceContext)
{
	deviceContext.ClearDephtStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void DepthStencilView::destroy()
{
}

