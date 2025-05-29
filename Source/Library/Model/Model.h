#pragma once
#include "Common.h"

#include "Renderer/Renderable.h"          // base-class & BasicMeshEntry
#include "Renderer/DataTypes.h"           // SimpleVertex
#include "Texture/Texture.h"             // Texture wrapper

struct aiScene;
struct aiMesh;
struct aiMaterial;

/**
 *  Model
 *  -----
 *  Loads a file via Assimp, creates GPU buffers, textures and material data.
 */
class Model final : public Renderable
{
public:
    explicit Model(const std::filesystem::path& filePath);

    /* Renderable interface */
    HRESULT Initialize(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext) override;
    void     Update(FLOAT /*deltaTime*/) override {}          // static model

    UINT GetNumVertices() const override { return static_cast<UINT>(m_aVertices.size()); }
    UINT GetNumIndices()  const override { return static_cast<UINT>(m_aIndices.size()); }

protected:                     /* Renderable helpers */
    const SimpleVertex* getVertices() const override { return m_aVertices.data(); }
    const WORD* getIndices() const override { return m_aIndices.data(); }

private:                      /* Assimp processing pipeline */
    HRESULT initFromScene(
        _In_ ID3D11Device* pDevice,
        _In_ ID3D11DeviceContext* pImmediateContext,
        _In_ const aiScene* pScene,
        _In_ const std::filesystem::path& filePath);

    void    countVerticesAndIndices(UINT& outVertices,
        UINT& outIndices,
        const aiScene* pScene);

    void    reserveSpace(UINT vertices, UINT indices);
    void    initAllMeshes(const aiScene* pScene);
    void    initSingleMesh(const aiMesh* pMesh);

    HRESULT initMaterials(ID3D11Device* pDevice,
        ID3D11DeviceContext* pImmediateContext,
        const aiScene* pScene,
        const std::filesystem::path& filePath);

    HRESULT loadTextures(ID3D11Device* pDevice,
        ID3D11DeviceContext* pImmediateContext,
        const std::filesystem::path& parentDir,
        const aiMaterial* pMaterial,
        UINT                      index);

    HRESULT loadDiffuseTexture(ID3D11Device* device,
        ID3D11DeviceContext* context,
        const std::filesystem::path& parentDir,
        const aiMaterial* pMaterial,
        UINT                index);

    HRESULT loadSpecularTexture(ID3D11Device* device,
        ID3D11DeviceContext* context,
        const std::filesystem::path& parentDir,
        const aiMaterial* pMaterial,
        UINT               index);

    void    loadColors(const aiMaterial* pMaterial, UINT index);

private:
    std::filesystem::path       m_filePath;   // model file on disk
    std::vector<SimpleVertex>   m_aVertices;  // aggregated vertex buffer
    std::vector<WORD>           m_aIndices;   // aggregated index buffer
};