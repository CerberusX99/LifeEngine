#include "Transform.h"

void
Transform::init() {
    m_fScaleNum = 0.5f;
    m_fRotateNum = 0.0002f;
    m_fSpeed = 100.0f;

    m_v3Scale.x = 0.5f;
    m_v3Scale.y = 0.5f;
    m_v3Scale.z = 0.5f;
}

void
Transform::update() {
}

void
Transform::render() {
}

void
Transform::destroy() {
}