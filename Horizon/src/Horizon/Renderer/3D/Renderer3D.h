#pragma once

#include "..\OrthographicCamera.h"
#include "..\PerspectiveCamera.h"
#include "hzpch.h"
#include <glm/glm.hpp>

#include "../Model/Model.h"

namespace Horizon
{
	// struct MeshVertex
	// {
	// 	glm::vec3 Position;
	// 	glm::vec3 Normal;
	// 	glm::vec2 TexCoord;
	// 	glm::vec4 Color;
	// };

	class Model;

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

		static void DrawMesh(const Ref<Horizon::Model>& model);

		// static void DrawCube((const Ref<Horizon::Model>& model, const glm::vec3& position, const glm::vec3& size);
	};
}