#pragma once

#include "..\OrthographicCamera.h"
#include "hzpch.h"

namespace Horizon
{
	class Renderer3D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec3& position, glm::vec3& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec4& position, glm::vec3& size, const glm::vec4& color);
	};
}