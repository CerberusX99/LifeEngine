#include "Material.h"
#include "Device.h"
#include "DeviceContext.h"
#include "Texture.h"

void
Material::init(Device device,
    DeviceContext& deviceContext,
    std::unordered_map<TextureType, std::string> textureNames) {
    // Inicializamos las texturas
    for (auto it = textureNames.begin(); it != textureNames.end(); ++it) {
        if (it->first == TextureType::ALBEDO) {
            m_materialBuffer.albedo.init(device, it->second);
        }
        if (it->first == TextureType::NORMAL) {
            m_materialBuffer.normal.init(device, it->second);
        }
        if (it->first == TextureType::METALLIC) {
            m_materialBuffer.metallic.init(device, it->second);
        }
        if (it->first == TextureType::ROUGHNESS) {
            m_materialBuffer.roughness.init(device, it->second);
        }
    }

    // Crear el buffer constante del material
    m_constantBuffer->init(device, sizeof(MaterialBuffer));
}

void
Material::render(DeviceContext& deviceContext) {
    // Enlazar el buffer constante de material al pipeline de rendering
    m_constantBuffer->render(deviceContext, 0, 1);

    // Enlazar las texturas
    m_materialBuffer.albedo.render(deviceContext, 0, 1);
    m_materialBuffer.normal.render(deviceContext, 1, 1);
    m_materialBuffer.metallic.render(deviceContext, 2, 1);
    m_materialBuffer.roughness.render(deviceContext, 3, 1);
}
