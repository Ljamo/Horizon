#pragma once

#include "..\OrthographicCamera.h"
#include "..\PerspectiveCamera.h"
#include "hzpch.h"

#include "../texture.h"
// #include "../Mesh/Mesh.h"

namespace Horizon
{
	// struct MeshVertex
	// {
	// 	glm::vec3 Position;
	// 	glm::vec3 Normal;
	// 	glm::vec2 TexCoord;
	// 	glm::vec4 Color;
	// };

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();

		static void Flush();

		// Primitives
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);

		static void DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const glm::vec4& color);
		static void DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const Ref<Texture2D>& texture);

		// New overloads using Mesh and style parameters.
		static void DrawMesh(const Model& model, const glm::vec3& position,
			const glm::vec3& size, const glm::vec3& rotation,
			const glm::vec4& color);

		static void DrawMesh(const Model& model, const glm::vec3& position,
			const glm::vec3& size, const glm::vec3& rotation,
			const Ref<Texture2D>& texture);
		// TODO Implement others such as:
		// Mesh importer
		// scene importer?
		
		// Draw mesh
	private:
		static void DrawMesh(const glm::mat4& transform, const MeshVertex* vertices, uint32_t vertexCount,
			const uint32_t* indices, uint32_t indexCount);
	};
}