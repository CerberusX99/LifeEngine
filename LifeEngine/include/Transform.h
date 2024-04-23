#pragma once
#include "Prerequisites.h"
#include "Commons.h"

class
    Transform {
public:
    Transform() = default;
    ~Transform() {};
          void
        init();

    void
        update();

    void
        render();

    void
        destroy();

public:
   
    Vector3     m_v3Position;
    Vector3        m_v3Scale;
    Vector3        m_v3Rotate;
    float        m_fScaleNum;
    float        m_fRotateNum;
    float        m_fSpeed;
};