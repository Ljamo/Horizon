#include "hzpch.h"
#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

#include <stb_image.h>

namespace Horizon
{
    int Model::s_ModelIDCounter = 0;

    void Model::LoadModel()
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            m_Path,
            aiProcess_Triangulate |
            aiProcess_GenNormals |    // Generate normals if missing
            // aiProcess_FlipUVs |       // Flip UVs for OpenGL
            aiProcess_CalcTangentSpace // Required for normal maps
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            HZ_CORE_ERROR("Assimp Error: {0}", importer.GetErrorString());
            return;
        }

        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }
        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Ref<Texture2D>> textures;

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            // texture coordinates
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoord = vec;
            }
            else
                vertex.TexCoord = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<Ref<Texture2D>> diffuseMaps = LoadMaterialTextures(
            material, aiTextureType_DIFFUSE, "texture_diffuse", scene
        );
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // If no texture was loaded, create a default pink texture.
        if (diffuseMaps.empty()) 
        {
            Ref<Texture2D> defaultTex = Texture2D::Create(1, 1);
            uint32_t pink = 0xFFFF00FF; // RGBA pink
            defaultTex->SetData(&pink, sizeof(uint32_t));
            textures.push_back(defaultTex);
        }
        return Mesh(vertices, indices, textures);
    }

    std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
        const std::string& typeName, const aiScene* scene)
    {
        // Get the number of textures for this type from the material
        int textureCount = mat->GetTextureCount(type);
        HZ_CORE_WARN("Material has {0} {1} texture(s)", textureCount, typeName);
        std::vector<Ref<Texture2D>> textures;

        for (int i = 0; i < textureCount; i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // Log the texture identifier from Assimp:
            HZ_CORE_WARN("Texture identifier: {0}", str.C_Str());

            // Check for embedded textures:
            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
            if (embeddedTexture)
            {
                if (embeddedTexture->mHeight == 0)
                {
                    try {
                        Ref<Texture2D> texture = Texture2D::CreateFromMemory(
                            reinterpret_cast<unsigned char*>(embeddedTexture->pcData),
                            embeddedTexture->mWidth);
                        textures.push_back(texture);
                        m_LoadedTextures.push_back(texture);
                        continue; // Continue to next texture if this one is embedded
                    }
                    catch (...)
                    {
                        HZ_CORE_ERROR("Failed to load embedded texture: {0}", str.C_Str());
                    }
                }
            }

            bool skip = false;

            // Adjust the texture identifier if an unwanted folder prefix is present.
            std::string textureIdentifier = str.C_Str();
            const std::string folderPrefix = "Ico.fbm\\";
            if (textureIdentifier.find(folderPrefix) == 0)
            {
                textureIdentifier = textureIdentifier.substr(folderPrefix.length());
            }

            // Construct full path for the texture file.
            std::filesystem::path texturePath = std::filesystem::path(m_Path).parent_path() / textureIdentifier;
            std::string fullPath = texturePath.string();

            // Log out the texture search path.
            HZ_CORE_WARN("Looking for texture at: {0}", fullPath);

            // Verify existence of the file
            if (std::filesystem::exists(fullPath))
            {
                HZ_CORE_INFO("Texture file exists at: {0}", fullPath);
            }
            else
            {
                HZ_CORE_ERROR("Texture file NOT found at: {0}", fullPath);
            }

            // Check if the texture was already loaded.
            for (uint32_t j = 0; j < m_LoadedTextures.size(); j++)
            {
                if (m_LoadedTextures[j]->GetPath() == fullPath)
                {
                    textures.push_back(m_LoadedTextures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                try {
                    Ref<Texture2D> texture = Texture2D::Create(fullPath);
                    textures.push_back(texture);
                    m_LoadedTextures.push_back(texture);
                }
                catch (...)
                {
                    HZ_CORE_ERROR("Failed to load texture: {0}", fullPath);
                }
            }
        }
        return textures;
    }

    void Model::UpdateModelMatrix()
    {
        m_ModelMatrix = glm::mat4(1.0f);
        m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
    }
}
