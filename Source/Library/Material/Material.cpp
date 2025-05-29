#include "Material/Material.h"

Material::Material()
    : pDiffuse(nullptr)
    , pSpecular(nullptr)
    // Initialize colors to some defaults
    , AmbientColor(0.1f, 0.1f, 0.1f)   // Dark grey ambient
    , DiffuseColor(1.0f, 1.0f, 1.0f)   // White diffuse
    , SpecularColor(0.5f, 0.5f, 0.5f)  // Grey specular
{
}