#pragma once

#include "Horizon.h"

// #include <ofbx.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <Horizon/Audio/MaterialProperties.h>

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

	struct Mesh
	{
	public:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<Ref<Texture2D>> m_Textures;

		MaterialAudioProperties m_AudioProperties;

		Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Texture2D>>& textures)
			: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
		{
		}

		void SetAudioProperties(MaterialAudioProperties& audioProperties)
		{
			m_AudioProperties = audioProperties;
		}

		const MaterialAudioProperties& GetAudioProperties() const { return m_AudioProperties; }

	private:
		//int m_MeshID;
		// static int s_MeshIDCounter;
	};
	
	class Model
	{
	public:
		Model(const std::string& path)
			: m_Path(path), m_ModelID(s_ModelIDCounter)
		{
			s_ModelIDCounter++;
			LoadModel();
		}

		// Model(){}
	
		const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
		const std::string& GetPath() const { return m_Path; }
		const std::vector<Ref<Texture2D>>& GetLoadedTextures() const { return m_LoadedTextures; }
		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetScale() const { return m_Scale; }
		const glm::vec3& GetRotation() const { return m_Rotation; }
		const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }

		void SetPosition(const glm::vec3 position) { m_Position = position; UpdateModelMatrix(); }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; UpdateModelMatrix(); }
		void SetRotation(const glm::vec3 rotation) { m_Rotation = rotation; UpdateModelMatrix(); }

		void Rotate(const glm::vec3 rotation) { m_Rotation += rotation; UpdateModelMatrix(); }
		void Translate(const glm::vec3 distance) { m_Position += distance; UpdateModelMatrix(); }
		void Scale(const glm::vec3 scale) { m_Scale *= scale; UpdateModelMatrix(); }

	private:
		const std::string m_Path;
		std::vector<Mesh> m_Meshes;
		// std::unordered_map<std::string, Ref<Texture2D>> m_LoadedTextures;
		std::vector<Ref<Texture2D>> m_LoadedTextures;
		
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Scale = glm::vec3(1.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f);

		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

		void LoadModel();
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, 
			const std::string& typeName, const aiScene* scene);
		
		void UpdateModelMatrix();

	private:
		int m_ModelID;
		static int s_ModelIDCounter;
	};

}