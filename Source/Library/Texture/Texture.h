#pragma once

#include "Common.h"

class Texture
{
public:
    Texture(const std::filesystem::path& filePath);

    HRESULT Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext);

    ComPtr<ID3D11ShaderResourceView>& GetTextureResourceView();
    ComPtr<ID3D11SamplerState>& GetSamplerState();
    const std::filesystem::path& GetFilePath() const;

private:
    std::filesystem::path           m_filePath;
    ComPtr<ID3D11ShaderResourceView> m_textureRV;
    ComPtr<ID3D11SamplerState>       m_samplerLinear;
};