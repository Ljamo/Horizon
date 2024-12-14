#include "2D/Sandbox2D.h"
#include "3D/Sandbox3D.h"
#include "Horizon/Core/EntryPoint.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Horizon/Core/Timer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/glm/gtc/type_ptr.hpp>


class ExampleLayer : public Horizon::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray = Horizon::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Horizon::Ref<Horizon::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Horizon::VertexBuffer::Create(vertices, sizeof(vertices)));
		Horizon::BufferLayout layout = {
			{ Horizon::ShaderDataType::Float3, "a_Position" },
			{ Horizon::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Horizon::Ref<Horizon::IndexBuffer> indexBuffer;
		indexBuffer.reset(Horizon::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Horizon::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Horizon::Ref<Horizon::VertexBuffer> squareVB;
		squareVB.reset(Horizon::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Horizon::ShaderDataType::Float3, "a_Position" },
			{ Horizon::ShaderDataType::Float2, "a_TextCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Horizon::Ref<Horizon::IndexBuffer> squareIB;
		squareIB.reset(Horizon::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Horizon::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = Horizon::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Horizon::Texture2D::Create("assets/textures/Checkerboard.png");
		m_TransparentTestTexture = Horizon::Texture2D::Create("assets/textures/BlendTest.png");

		std::dynamic_pointer_cast<Horizon::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Horizon::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Horizon::Timestep ts) override
	{
		// Update Function
		m_CameraController.OnUpdate(ts);

		// Rendering functions
		Horizon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Horizon::RenderCommand::Clear();

		Horizon::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<Horizon::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Horizon::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				/*if (x % 2 == 0)
					m_FlatColorShader->UploadUniformFloat4("u_Color", redColor);
				else
					m_FlatColorShader->UploadUniformFloat4("u_Color", blueColor);*/
				Horizon::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Horizon::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_TransparentTestTexture->Bind();
		Horizon::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		// Triangle
		//Horizon::Renderer::Submit(m_Shader, m_VertexArray);

		Horizon::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Horizon::Event& e) override
	{
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == Horizon::EventType::WindowResize)
		{
			auto& re = (Horizon::WindowResizeEvent&)e;

			//Already calculated
			// float zoom = (float)re.GetWidth() / 1280.0f;
			// m_CameraController.SetZoomLevel(zoom);
		}
	}

private:
	Horizon::ShaderLibrary m_ShaderLibrary;
	Horizon::Ref<Horizon::Shader> m_Shader;
	Horizon::Ref<Horizon::VertexArray> m_VertexArray;

	Horizon::Ref<Horizon::Shader> m_FlatColorShader;
	Horizon::Ref<Horizon::VertexArray> m_SquareVA;

	Horizon::Ref<Horizon::Texture2D> m_Texture, m_TransparentTestTexture;

	Horizon::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Horizon::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox3D());
	}

	~Sandbox()
	{

	}
};

Horizon::Application* Horizon::CreateApplication()
{
	return new Sandbox();
}