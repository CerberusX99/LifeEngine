#pragma once
#include "Prerequisites.h"
class DeviceContext {
public:
	DeviceContext()=default;
	~DeviceContext() { SAFE_RELEASE(m_deviceContext); };

	void 
		init();

	void
		update();

	void
		render();

	void
		Destroy();
	void
		ClearDephtStencilView(ID3D11DepthStencilView* pDepthStencilView,
								unsigned int ClearFlags,
								float Depth,
								unsigned int Stencil);
	public :
		ID3D11DeviceContext* m_deviceContext = nullptr;

};
