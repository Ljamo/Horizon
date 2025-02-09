#pragma once

#include "Horizon.h"
#include "Mesh.h"

namespace Horizon
{

class Model
{
	public:
		Model(const std::string& path)
			: m_Path(path)
		{
			LoadModel(path);
		}
	private:
		std::string m_Path;
		std::vector<Mesh> m_Meshes;
	
		void LoadModel(std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessNode(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadTextures(aiMaterial* material, aiTextureType type, std::string& typename)
	};
}