
#include "CCamera.h"
#include "Device.h"
#include "DeviceContext.h"

void
CCamera::init(Device device) {

    if (device.m_device == nullptr) {
        WARNING("ERROR: Camera::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Device device] \n");
        exit(1);
    }

    HRESULT hr = S_OK;

    //Create the constant buffers
    D3D11_BUFFER_DESC CamBufferDesc;
    memset(&CamBufferDesc, 0, sizeof(CamBufferDesc));
    CamBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    CamBufferDesc.ByteWidth = sizeof(Camera);
    CamBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    CamBufferDesc.CPUAccessFlags = 0;
    hr = device.CreateBuffer(&CamBufferDesc, nullptr, &m_camera);

    if (FAILED(hr)) {
        WARNING("ERROR: Camera::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR CreateBuffer()] \n");
        exit(1);
    }
}

void
CCamera::update(DeviceContext& deviceContext, Camera cam) {
    deviceContext.UpdateSubresource(m_camera, 0, nullptr, &cam, 0, 0);
}

void
CCamera::render(DeviceContext& deviceContext) {
    deviceContext.VSSetConstantBuffers(0, 1, &m_camera);
}

void
CCamera::destroy() {
    SAFE_RELEASE(m_camera);
}