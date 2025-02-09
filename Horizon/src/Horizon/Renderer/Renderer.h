#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Shader.h"

namespace Horizon {

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void SetDimension(RendererAPI::Dimension dimension);
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		inline static RendererAPI::Dimension GetDimension() { return RendererAPI::GetDimension(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			float ts;
		};

		static SceneData* s_SceneData;
	};


}