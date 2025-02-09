#include "hzpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "2D/Renderer2D.h"
#include "3D/Renderer3D.h"

namespace Horizon {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		switch (RendererAPI::GetDimension())
		{
			case RendererAPI::Dimension::_2D:
			{
				Renderer2D::Init();
				//HZ_INFO("Initialized Renderer2D.");
				break;
			}
			case RendererAPI::Dimension::_3D:
			{
				Renderer3D::Init();
				//HZ_INFO("Initialized Renderer3D.");
				break;
			}
			default:
			{
				HZ_ERROR("No Game Dimension was chosen.");
				break;
			}
		}
	}

	void Renderer::SetDimension(RendererAPI::Dimension dimension)
	{
		switch (RendererAPI::GetDimension())
		{
			case RendererAPI::Dimension::_2D:
			{
				Renderer2D::Shutdown();
				//HZ_INFO("Shutdown Renderer2D.");
				break;
			}
			case RendererAPI::Dimension::_3D:
			{
				Renderer3D::Shutdown();
				//HZ_INFO("Shutdown Renderer3D.");
				break;
			}
			default:
			{
				HZ_ERROR("Could not shutdown renderer.");
				break;
			}
		}
		RendererAPI::SetDimension(dimension);
		Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
 		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}