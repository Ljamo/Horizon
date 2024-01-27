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

	Horizon::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Horizon::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.8f, 0.2f, 0.3f, 1.0f});
	Horizon::Renderer2D::EndScene();

	//std::dynamic_pointer_cast<Horizon::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<Horizon::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
};

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

	ImGui::Begin("Viewport");

	ImGui::End();
};

void Sandbox2D::OnEvent(Horizon::Event& e)
{
	m_CameraController.OnEvent(e);
};
