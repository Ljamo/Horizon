#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Horizon::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 
		 0.5f, -0.5f, 0.0f, 
		 0.5f,  0.5f, 0.0f, 
		-0.5f,  0.5f, 0.0f
	};

	Horizon::Ref<Horizon::VertexBuffer> squareVB;
	squareVB.reset(Horizon::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ Horizon::ShaderDataType::Float3, "a_Position" }
	});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Horizon::Ref<Horizon::IndexBuffer> squareIB;
	squareIB.reset(Horizon::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);


	m_FlatColorShader = Horizon::Shader::Create("assets/shaders/FlatColor.glsl");
};

void Sandbox2D::OnDetach()
{

};

void Sandbox2D::OnUpdate(Horizon::Timestep ts)
{
	// Update Function
	m_CameraController.OnUpdate(ts);

	// Rendering functions
	Horizon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Horizon::RenderCommand::Clear();

	Horizon::Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	std::dynamic_pointer_cast<Horizon::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Horizon::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Horizon::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Horizon::Renderer::EndScene();
};

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
};

void Sandbox2D::OnEvent(Horizon::Event& e)
{
	m_CameraController.OnEvent(e);
};
