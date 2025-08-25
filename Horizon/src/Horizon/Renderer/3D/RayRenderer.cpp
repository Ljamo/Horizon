#include "hzpch.h"
#include "RayRenderer.h"

#include "../VertexArray.h"
#include "../Shader.h"
#include "../RenderCommand.h"
#include "../Model/Model.h"
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include <map>

namespace Horizon
{

	struct RayRendererData
	{
		Ref<Shader> RayShader;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;


		Renderer3D::Statistics Stats;

		glm::mat4 ViewProjection = glm::mat4(1.0f);

		float QuadVertices[8] = {
			// positions (x, y)
			-1.0f, -1.0f,  // bottom left
			 1.0f, -1.0f,  // bottom right
			 1.0f,  1.0f,  // top right
			-1.0f,  1.0f   // top left
		};

		void* VertexBufferPtr = nullptr;
	};

	static RayRendererData s_RayRenderData;

	void RayRenderer::Init()
	{
		HZ_PROFILE_FUNCTION();

		s_RayRenderData.QuadVertexArray = VertexArray::Create();

		s_RayRenderData.VertexBufferPtr = &s_RayRenderData.QuadVertices;

		s_RayRenderData.QuadVertexBuffer = VertexBuffer::Create(sizeof(s_RayRenderData.QuadVertices));
		s_RayRenderData.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Position" }
			});
		s_RayRenderData.QuadVertexArray->AddVertexBuffer(s_RayRenderData.QuadVertexBuffer);

		unsigned int QuadIndices[6] = {
			0, 1, 2,
			2, 3, 0 
		};

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(QuadIndices, 6);
		s_RayRenderData.QuadVertexArray->SetIndexBuffer(quadIB);


		s_RayRenderData.RayShader = Shader::Create("assets/shaders/RayShader.glsl");
	}

	void RayRenderer::Shutdown()
	{

	}

	void RayRenderer::BeginScene(const PerspectiveCamera& camera)
	{
		s_RayRenderData.RayShader->Bind();
		s_RayRenderData.ViewProjection = camera.GetViewProjectionMatrix();
		EndScene();
	}

	void RayRenderer::EndScene()
	{
		HZ_PROFILE_FUNCTION();

		uint32_t dataSize = sizeof(s_RayRenderData.QuadVertices);
		s_RayRenderData.QuadVertexBuffer->SetData(s_RayRenderData.VertexBufferPtr, dataSize);

		RenderCommand::DrawIndexed(s_RayRenderData.QuadVertexArray, 6);
	}

	void RayRenderer::DrawScreenQuad()
	{
		RenderCommand::DrawByGPU(2);
	}

}