#pragma once
#include "Prerequisites.h"
#include "Buffer.h"
class Device;
class DeviceContext;

class
    Model {
public:
    Model() = default;
    ~Model() {};

    void
        init(Device device, Mesh loadData);

    void
        update(DeviceContext& deviceContext, std::string wName);

    void
        render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumBuffers);

    void
        destroy();

    void
        ui(std::string wName);

    /*
    void
      setTransform(Transform t);

    Transform
      getTransform();
    */

    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }
    void setStatic(bool _static) { m_static = _static; }

public:
    Buffer                        g_vertexBuffer;
    Buffer                         g_indexBuffer;
    Buffer                      g_modelBuffer;
    Mesh m_loadData;
    //Transform m_transform;
    bool m_active = true;
    bool m_static = false;
};