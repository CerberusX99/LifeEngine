#pragma once
#include "Prerequisites.h"
#include "Commons.h"

class
    Transform {
public:
    Transform() = default;
    ~Transform() {};

    /* Initialize transform's variables

Now we update data in Tutotial07 --> WndProc
Increase or decrease scale with m_fSpeed * g_Time.m_fDeltaTime
Rotate only in axis Y with g_Time.m_fDeltaTime
*/
    void
        init();

    void
        update();

    void
        render();

    void
        destroy();

public:
    //CHECK OUT: I need to create a Vector escalar and rotation
    Vector3     m_v3Position;
    Vector3        m_v3Scale;
    Vector3        m_v3Rotate;
    float        m_fScaleNum;
    float        m_fRotateNum;
    float        m_fSpeed;
};