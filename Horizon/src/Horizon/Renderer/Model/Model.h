#pragma once

#include "Horizon.h"

// #include <ofbx.h>
#include <memory>
#include <cstdint>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

namespace Horizon
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
		float TexIndex;
		// glm::vec4 Color;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices,
			std::vector<Ref<Texture2D>> textures);

		void SetVertices(std::vector<Vertex> vertices);
		void SetIndices(std::vector<uint32_t> indices);
		void SetTextures(std::vector<Ref<Texture2D>> textures);

		const std::vector<Vertex>& GetVertices() { return m_Vertices; }
		const std::vector<uint32_t>& GetIndices() { return m_Indices; }
		const std::vector<Ref<Texture2D>>& GetTextures() { return m_LoadedTextures; }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		// Later on will change Texture2D to id type of texture, e.g. normal, roughness...
		std::vector<Ref<Texture2D>> m_LoadedTextures;
	};

	class Model
	{
	public:
		Model(aiNode* node, const aiScene* scene, const std::string& path)
			: m_Path(path)
		{
			LoadMesh(node, scene);
		}

		const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }
	private:
		void LoadMesh(aiNode* node, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);

		std::string m_Path;

		std::vector<std::shared_ptr<Mesh>> m_Meshes;
	};

	class Object
	{
	public:
		Object(std::string path);
		
		std::vector<Model>& GetModels() { return m_Models; }
		// const glm::mat4& GetMatrix();
	private:
		void ImportModels();
		void GetModelNodes(aiNode* node, std::vector<aiNode*>& out);

		std::string m_Path;
		std::vector<Model> m_Models;

		// glm::mat4 m_ObjectMatrix = glm::mat4(1.0f);
		// glm::vec3 m_Rotation;
		// glm::vec3 m_Position;
	};

}