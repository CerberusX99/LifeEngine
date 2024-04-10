#pragma once
#include "Prerequisites.h"
#include "Texture.h"
#include "Buffer.h"

class Device;
class DeviceContext;

struct
    MaterialBuffer {
    Texture albedo;
    Texture normal;
    Texture metallic;
    Texture ambientOcclusion;
    Texture roughness;
};

enum TextureType {
    NONE = 0,
    ALBEDO,
    NORMAL,
    METALLIC,
    ROUGHNESS,
    SPECULAR,
    AO
};

class
    Material {
public:
    Material() = default;
    ~Material() = default;


    void
        init(Device device,
            DeviceContext& deviceContext,
            std::unordered_map<TextureType, std::string> textureNames);

    void
        update();

    void
        render(DeviceContext& deviceContext);

    void
        destroy();
private:
    MaterialBuffer m_materialBuffer;
    Buffer* m_constantBuffer;
};
