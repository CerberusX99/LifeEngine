#pragma once
#include "Prerequisites.h"
class Device {
public:
		Device()=default;
		~Device()=default;

		void 
			init();

		void
			update();

		void
			render();

		void
			Destroy();
		 HRESULT CreateRenderTargetView(ID3D11Resource* pResource,
										const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
										ID3D11RenderTargetView** ppRTView);
		 virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
														   const D3D11_SUBRESOURCE_DATA* pInitialData,
														   ID3D11Texture2D** ppTexture2D);
		 virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(ID3D11Resource* pResource,
															      const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
																  ID3D11DepthStencilView** ppDepthStencilView);
public:
	ID3D11Device * m_device=nullptr;
};
