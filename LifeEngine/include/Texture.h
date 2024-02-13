#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

// Texture class for managing textures
class Texture {
public:
    Texture() = default;
    ~Texture();

    // Initialize texture from a file
    void init(Device device, std::string textureName);

    // Initialize texture with specified dimensions and format
    void init(Device device,
        unsigned width,
        unsigned height,
        DXGI_FORMAT Format,
        unsigned int BlindFlags);

    // Update texture (if needed)
    void update();

    // Render the texture
    void render(DeviceContext& deviceContext, unsigned int StartSlot);

    // Destroy the texture and release resources
    void destroy();

public:
    ID3D11Texture2D* m_texture = nullptr; // Handle to texture resource
    ID3D11ShaderResourceView* m_textureFromImg; // Handle to image data for texture resource
};
