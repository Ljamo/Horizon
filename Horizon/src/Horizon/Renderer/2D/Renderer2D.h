#pragma once

#include ".\Horizon\Renderer\OrthographicCamera.h"
#include "hzpch.h"

namespace Horizon
{
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2 size, glm::vec4 color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2 size, glm::vec4 color);
	};
}