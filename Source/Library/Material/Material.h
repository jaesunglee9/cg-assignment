#pragma once

#include "Common.h" // For XMFLOAT3
#include "Texture/Texture.h" // Include Texture.h created in Step 1
#include <memory> // For std::shared_ptr

class Material
{
public:
    Material();
    ~Material() = default;

    // Rule of 5/3: if you have custom destructor, copy/move, you usually need all
    // For now, deleting copy and allowing default move for simplicity with shared_ptr
    Material(const Material& other) = delete;
    Material& operator=(const Material& other) = delete;
    Material(Material&& other) = default;
    Material& operator=(Material&& other) = default;

    std::shared_ptr<Texture> pDiffuse;
    std::shared_ptr<Texture> pSpecular;

    XMFLOAT3 AmbientColor;
    XMFLOAT3 DiffuseColor;
    XMFLOAT3 SpecularColor; // Assuming the second AmbientColor on slide 25 was a typo for SpecularColor
    // FLOAT    Shininess; // You might add shininess/specular power here later
};