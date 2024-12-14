#pragma once

#include "..\OrthographicCamera.h"
#include "..\PerspectiveCamera.h"
#include "hzpch.h"

#include "../texture.h"

namespace Horizon
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);
	};
}