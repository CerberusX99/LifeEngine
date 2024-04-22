#pragma once
#include "Prerequisites.h"
#include "Commons.h"

class
    Device;

class
    DeviceContext;


class
    CCamera {
public:
    CCamera() = default;
    ~CCamera() {};


    void
        init(Device device);


    void
        update(DeviceContext& deviceContext, Camera cam);


    void
        render(DeviceContext& deviceContext);
    void
        destroy();

public:
    ID3D11Buffer* m_camera = nullptr;

};