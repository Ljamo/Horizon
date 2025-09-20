#pragma once

#include "..\PerspectiveCamera.h"
#include "hzpch.h"
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <array>
#include <string>
#include <memory>

// #include "../Model/Model.h"

namespace Horizon
{
	class RayRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const PerspectiveCamera& camera, glm::vec2 dimensions);
		static void EndScene();
		static void DrawScreenQuad();

		// static void DrawCube((const Ref<Horizon::Model>& model, const glm::vec3& position, const glm::vec3& size);

	private:
	};
}