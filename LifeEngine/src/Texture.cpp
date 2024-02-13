#include "Texture.h"
#include "Device.h"
#include "DeviceContext.h"

// Destructor for Texture class
Texture::~Texture()
{
    // Release the texture resources if they are not nullptr
    if (m_texture != nullptr) {
        SAFE_RELEASE(m_texture);
    }
    else if (m_textureFromImg != nullptr) {
        SAFE_RELEASE(m_textureFromImg);
    }
}

// Initialize the texture from an image file
void Texture::init(Device device, std::string textureName)
{
    // Check if the device is valid
    if (device.m_device == nullptr) {
        WARNING("ERROR: Texture::init : Error in data from params [CHECK FOR Device device]\n");
        exit(1);
    }
    HRESULT hr = S_OK;
    // Load the texture from file
    hr = D3DX11CreateShaderResourceViewFromFile(device.m_device,
        textureName.c_str(),
        nullptr,
        nullptr,
        &m_textureFromImg,
        nullptr);
    if (FAILED(hr)) {
        // Handle error if failed to load texture
        WARNING("ERROR: Texture::init : Error in data from params [CHECK FOR string textureName -> Verify correct texture name in Filepath]\n");
        exit(1);
    }
}

// Initialize the texture with specified parameters
void Texture::init(Device device,
    unsigned int width,
    unsigned int height,
    DXGI_FORMAT Format,
    unsigned int BindFlags) {
    // Check if the device is valid and width/height are positive
    if (device.m_device == nullptr || width <= 0 || height <= 0) {
        WARNING("ERROR: Texture::init : Error in data from params [CHECK FOR Device device or unsigned int width/height]\n");
        exit(1);
    }
    HRESULT hr = S_OK;

    D3D11_TEXTURE2D_DESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = Format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = BindFlags;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    // Create texture based on specified bind flags
    if (BindFlags == D3D11_BIND_DEPTH_STENCIL || BindFlags == D3D11_BIND_RENDER_TARGET) {
        hr = device.CreateTexture2D(&desc, nullptr, &m_texture);
    }

    if (m_texture == nullptr) {
        // Handle error if failed to create texture
        WARNING("ERROR: Texture::init : Error in data from params [CHECK FOR m_texture]\n");
        exit(1);
    }
    else if (FAILED(hr)) {
        // Handle error if failed to create texture
        WARNING("ERROR: Texture::init : Error in data from params [CHECK FOR CreateTexture2D]\n");
        exit(1);
    }
}

// Render the texture
void Texture::render(DeviceContext& deviceContext, unsigned int StarSlot) {
    // Check if the texture from image is not nullptr
    if (m_textureFromImg != nullptr) {
        // Set shader resources and render
        ID3D11ShaderResourceView* nullSRV[] = { nullptr };
        deviceContext.PSSetShaderResources(StarSlot, 1, nullSRV);
        deviceContext.PSSetShaderResources(StarSlot, 1, &m_textureFromImg);
    }
}

// Destroy the texture resources
void Texture::destroy() {
    // Release the texture resources if they are not nullptr
    if (m_texture != nullptr) {
        SAFE_RELEASE(m_texture);
    }
    else if (m_textureFromImg != nullptr) {
        SAFE_RELEASE(m_textureFromImg);
    }
}
