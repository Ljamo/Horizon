#pragma once

#include "Horizon.h"

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
		// glm::vec4 Color;
	};

	class Mesh
	{
	public:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<Ref<Texture2D>> m_Textures;

		Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Texture2D>>& textures)
			: m_Vertices(vertices), m_Indices(indices), m_Textures(textures) {}
	};

	class Model
	{
	public:
		Model(std::string& path)
			: m_Path(path)
		{
			LoadModel(path);
		}


	private:
		std::string m_Path;
		std::vector<Mesh> m_Meshes;

		void LoadModel(const std::string& path);
		// void ProcessNode(aiNode* node, const aiScene* scene);

	};
}