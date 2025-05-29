#include "Texture/Texture.h"

// IMPORTANT: You need to add WICTextureLoader.h and .cpp to your project.
// These are typically part of the DirectX Tool Kit (DirectXTK).
// Download from: github.com/microsoft/DirectXTK
// Add the .h and .cpp files to your project and adjust the include path below if necessary.
#include "WICTextureLoader11.h" // Assuming it's in a location accessible by your include paths

Texture::Texture(const std::filesystem::path& filePath)
    : m_filePath(filePath)
    , m_textureRV(nullptr)
    , m_samplerLinear(nullptr)
{
}

HRESULT Texture::Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext)
{
    // CreateShaderResourceView from WIC file
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        pDevice,
        pImmediateContext, // Context can be used for auto-generating mipmaps
        m_filePath.c_str(), // .c_str() on std::filesystem::path returns const wchar_t* on Windows
        nullptr,            // Do not auto-generate mips explicitly here (handled by loader if context is provided)
        m_textureRV.GetAddressOf()
    );

    if (FAILED(hr))
    {
        std::wstring errorMessage = L"ERROR: Failed to load texture \"";
        errorMessage += m_filePath.wstring();
        errorMessage += L"\"\n";
        OutputDebugString(errorMessage.c_str());
        return hr;
    }

    // Create the sampler state
    D3D11_SAMPLER_DESC sampDesc = {
        .Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
        .AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
        .ComparisonFunc = D3D11_COMPARISON_NEVER,
        .MinLOD = 0,
        .MaxLOD = D3D11_FLOAT32_MAX
    };

    hr = pDevice->CreateSamplerState(&sampDesc, m_samplerLinear.GetAddressOf());
    if (FAILED(hr))
    {
        OutputDebugString(L"ERROR: Failed to create sampler state for texture.\n");
        return hr;
    }

    return S_OK;
}

ComPtr<ID3D11ShaderResourceView>& Texture::GetTextureResourceView()
{
    return m_textureRV;
}

ComPtr<ID3D11SamplerState>& Texture::GetSamplerState()
{
    return m_samplerLinear;
}

const std::filesystem::path& Texture::GetFilePath() const
{
    return m_filePath;
}