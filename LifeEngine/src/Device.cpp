#include "Device.h";
void Device::init() {

}
void Device::update()
{
}
void Device::render()
{
}
void Device::Destroy()
{
	SAFE_RELEASE(m_device);
}

HRESULT 
Device::CreateRenderTargetView(ID3D11Resource* pResource,
									  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
									  ID3D11RenderTargetView** ppRTView)
{
	HRESULT hr = S_OK;
	if (pResource != nullptr || ppRTView != nullptr)
	{
		hr = m_device->CreateRenderTargetView(pResource, pDesc, ppRTView);
		WARNING("Device::CreateRenderTargetView : [CREATION OF RESOURCE : OK) ");
	}
	else {
		WARNING("ERROR: Device::CreateRenderTargetView : Error in data from params [CHECK FOR CREATION METHOD) ");
		exit(1);
	}
	return hr; 
}

HRESULT  
Device::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
{
	HRESULT hr= S_OK;
	if (pDesc != nullptr || ppTexture2D != nullptr)
	{
		hr = m_device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
		WARNING("Device::CreateTexture2D : [CREATION OF RESOURCE : OK) ");
	}
	else {
		WARNING("ERROR: Device::CreateTexture2D : Error in data from params [CHECK FOR CREATION METHOD) ");
		exit(1);
	}
	return hr;
}

HRESULT __stdcall Device::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
{
	HRESULT hr = S_OK;
	if (pResource != nullptr || ppDepthStencilView != nullptr)
	{
		hr = m_device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
		WARNING("ERROR: Device::CreateDepthStencilView : Error in data from params [CHECK FOR ID3D11Resource*pResource) ");
	}
	else if(ppDepthStencilView==nullptr) {
		WARNING("ERROR: Device::CreateDepthStencilView : Error in data from params [CHECK FOR ID3D11DepthStencilView**ppDepthStencilView) ");
		exit(1);
	}
	else {
		hr = m_device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
		WARNING("Device::CreateDepthStencilView : [CREATION OF RESOURCE : OK) ");
	}
	return hr;
}



