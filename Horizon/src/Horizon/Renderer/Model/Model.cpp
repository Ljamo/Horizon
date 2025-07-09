#include "hzpch.h"
#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

#include <stb_image.h>

namespace Horizon
{

    Object::Object(std::string path)
        : m_Path(path)
    {
        ImportModels();
    }

    void Object::ImportModels()
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(m_Path,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType |
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace);


        // If the import failed, report it
        if (!scene || !scene->mRootNode) {
            HZ_CORE_ERROR(importer.GetErrorString());
            return;
        }

        // Import models here
        std::vector<aiNode*> modelNodes;
        GetModelNodes(scene->mRootNode, modelNodes);

        m_Models.clear();
        m_Models.reserve(modelNodes.size());
        HZ_CORE_INFO("Found {} models in file", modelNodes.size());

        std::string path =
            std::filesystem::path(m_Path).parent_path().string();

        for (aiNode* nd : modelNodes)
            m_Models.emplace_back(Model(nd, scene, path));
    }

    void Object::GetModelNodes(aiNode* node, std::vector<aiNode*>& out)
    {
        if (node->mNumMeshes > 0)
            out.push_back(node);
        for (uint32_t i = 0; i < node->mNumChildren; ++i)
            GetModelNodes(node->mChildren[i], out);
    }
  
    void Model::LoadMesh(aiNode* node, const aiScene* scene)
    {
        for (uint32_t i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
            aiMaterial* aimaterial = scene->mMaterials[aimesh->mMaterialIndex];

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            // texture | texture path
            std::vector<Ref<Texture2D>> textures;

            vertices.reserve(aimesh->mNumVertices);
            for (uint32_t v = 0; v < aimesh->mNumVertices; ++v)
            {
                Vertex vert;
                vert.Position = glm::vec3(aimesh->mVertices[v].x, aimesh->mVertices[v].y, aimesh->mVertices[v].z);
                vert.Normal = aimesh->HasNormals()
                    ? glm::vec3(aimesh->mNormals[v].x, aimesh->mNormals[v].y, aimesh->mNormals[v].z)
                    : glm::vec3(0.0f);

                if (aimesh->mTextureCoords[0])
                    vert.TexCoord = { aimesh->mTextureCoords[0][v].x,
                                       aimesh->mTextureCoords[0][v].y };
                else
                    vert.TexCoord = glm::vec2(0.0f);

                vertices.push_back(vert);
            }

            for (uint32_t f = 0; f < aimesh->mNumFaces; ++f)
            {
                for (uint32_t k = 0; k < aimesh->mFaces[f].mNumIndices; ++k)
                {
                    indices.push_back(aimesh->mFaces[f].mIndices[k]);
                }
            }


            auto diffuseMaps = LoadMaterialTextures(aimaterial, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            auto specularMaps = LoadMaterialTextures(aimaterial, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            auto normalMaps = LoadMaterialTextures(aimaterial, aiTextureType_HEIGHT);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

            m_Meshes.push_back(std::make_shared<Mesh>(vertices, indices, textures));
            
        }


        // m_Mesh = std::make_shared<Mesh()>;
    }

    std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat,
            aiTextureType type)
    {
        std::vector<Ref<Texture2D>> textures;
        uint32_t count = mat->GetTextureCount(type);
        for (unsigned i = 0; i < count; ++i)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string filename = str.C_Str();
            std::string fullPath = m_Path + "/" + filename;
            textures.push_back(Texture2D::Create(fullPath));
        }
        return textures;
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices,
        std::vector<Ref<Texture2D>> textures)
    {
        SetVertices(vertices);
        SetIndices(indices);
        SetTextures(textures);
    }

    void Mesh::SetVertices(std::vector<Vertex> vertices)
    {
        m_Vertices = std::move(vertices);
    }

    void Mesh::SetIndices(std::vector<uint32_t> indices)
    {
        m_Indices = std::move(indices);
    }

    void Mesh::SetTextures(std::vector<Ref<Texture2D>> textures)
    {
        m_LoadedTextures = std::move(textures);
    }

}