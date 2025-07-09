#pragma once

#include "..\OrthographicCamera.h"
#include "..\PerspectiveCamera.h"
#include "hzpch.h"
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <array>
#include <string>
#include <memory>

#include "../Model/Model.h"

namespace Horizon
{

	class Model;
	class Object;
	class Mesh;

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();

		static void Flush();

		// static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);

		static void DrawObject(const Ref<Horizon::Object> object, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		// static void DrawCube((const Ref<Horizon::Model>& model, const glm::vec3& position, const glm::vec3& size);
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t MeshCount = 0;
			uint32_t VertexCount = 0;
			uint32_t IndexCount = 0;

			uint32_t GetTotalMeshCount() { return MeshCount; }
			uint32_t GetTotalVertexCount() { return VertexCount; }
			uint32_t GetTotalIndexCount() { return IndexCount; }
		};


		static void ResetStats();
		static Statistics GetStats();

	private:
		static void DrawModels(const std::vector<Model>& models, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		static void FlushAndReset();
		static void ImmediateDrawMesh(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform);
	};
}