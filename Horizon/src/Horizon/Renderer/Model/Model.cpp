#include "hzpch.h"
#include "Model.h"

// #include <ofbx.h>

namespace Horizon
{
    void Model::LoadModel()
    {
        Assimp::Importer importer;
        const const aiScene* scene = importer.ReadFile(m_Path, aiProcess_Triangulate |
            aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            HZ_CORE_ERROR("Error::Model::Assimp::%0", importer.GetErrorString());
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
                // tangent
                // vector.x = mesh->mTangents[i].x;
                // vector.y = mesh->mTangents[i].y;
                // vector.z = mesh->mTangents[i].z;
                // vertex.Tangent = vector;
                // // bitangent
                // vector.x = mesh->mBitangents[i].x;
                // vector.y = mesh->mBitangents[i].y;
                // vector.z = mesh->mBitangents[i].z;
                // vertex.Bitangent = vector;
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
        std::vector<Ref<Texture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        // vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // // 3. normal maps
        // std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // // 4. height maps
        // std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        // textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        
        return Mesh(vertices, indices, textures);
    }

    std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
		std::vector<Ref<Texture2D>> textures;
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            
			bool skip = false;
			for (uint32_t j = 0; j < m_LoadedTextures.size(); j++)
			{
				if (std::strcmp(m_LoadedTextures[j]->GetPath().c_str(), str.C_Str()) == 0)
				{
					textures.push_back(m_LoadedTextures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Ref<Texture2D> texture = Texture2D::Create(str.C_Str());
				textures.push_back(texture);
                m_LoadedTextures.push_back(texture);  // add to loaded textures
			}
        }
        return textures;
    }




    // void Model::LoadModel()
    // {
    //     std::ifstream file(m_Path, std::ios::binary | std::ios::ate);
    //     if (!file)
    //     {
    //         HZ_CORE_ERROR("Failed to open file: {0}", m_Path);
    //         return;
    //     }
    // 
    //     std::streamsize size = file.tellg();
    //     file.seekg(0, std::ios::beg);
    //     std::vector<char> buffer(size);
    //     if (!file.read(buffer.data(), size))
    //     {
    //         HZ_CORE_ERROR("Failed to read file: {0}", m_Path);
    //         return;
    //     }
    // 
    //     // Cast the buffer to ofbx::u8* and use (ofbx::u16)0 as load flag.
    //     const ofbx::IScene* scene = ofbx::load((const ofbx::u8*)buffer.data(), static_cast<int>(size), (ofbx::u16)0);
    //     if (!scene)
    //     {
    //         HZ_CORE_ERROR("Failed to parse FBX file: {0}", m_Path);
    //         return;
    //     }
    // 
    //     int meshCount = scene->getMeshCount();
    //     for (int i = 0; i < meshCount; ++i)
    //     {
    //         ofbx::Mesh* mesh = const_cast<ofbx::Mesh*>(scene->getMesh(i));
    //         ProcessFBXMesh(mesh);
    //     }
    // 
    //     // Clean up by destroying the scene.
    //     const_cast<ofbx::IScene*>(scene)->destroy();
    // }
    // 
    // void Model::ProcessFBXMesh(ofbx::Mesh* mesh)
    // {
	// 	std::vector<Vertex> vertices;
	// 	std::vector<uint32_t> indices;
	// 	std::vector<Ref<Texture2D>> textures;
	// 	int vertexCount = mesh->getGeometry()->getVertexCount();
	// 	const ofbx::Vec3* fbxVertices = mesh->getGeometry()->getVertices();
	// 	const ofbx::Vec3* fbxNormals = mesh->getGeometry()->getNormals();
	// 	const ofbx::Vec2* fbxUVs = mesh->getGeometry()->getUVs();
	// 	const ofbx::Vec4* fbxColors = mesh->getGeometry()->getColors();
	// 	for (int i = 0; i < vertexCount; ++i)
	// 	{
	// 		Vertex vertex;
	// 		vertex.Position = { fbxVertices[i].x, fbxVertices[i].y, fbxVertices[i].z };
	// 		vertex.Normal = { fbxNormals[i].x, fbxNormals[i].y, fbxNormals[i].z };
	// 		vertex.TexCoord = { fbxUVs[i].x, fbxUVs[i].y };
	// 		vertex.Color = { fbxColors[i].x, fbxColors[i].y, fbxColors[i].z, fbxColors[i].w };
	// 		vertices.push_back(vertex);
	// 	}
	// 	int indexCount = mesh->getGeometry()->getIndexCount();
	// 	const int* fbxIndices = mesh->getGeometry()->getFaceIndices();
	// 	for (int i = 0; i < indexCount; ++i)
	// 	{
	// 		indices.push_back(fbxIndices[i]);
	// 	}
	// 	m_Meshes.emplace_back(vertices, indices, textures);
    // }
}
