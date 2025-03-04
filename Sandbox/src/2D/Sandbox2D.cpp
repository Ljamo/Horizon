#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Timer.h"


//-------------------------------------------------------------------

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
	HZ_SET_2D();
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Horizon::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Horizon::Timestep ts)
{
	// Update
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		PROFILE_SCOPE("Renderer Prep");
		Horizon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Horizon::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Horizon::Renderer2D::BeginScene(m_CameraController.GetCamera());
		// Horizon::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0f), {0.8f, 0.2f, 0.3f, 1.0f});
		Horizon::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, {0.8f, 0.2f, 0.3f, 1.0f});
		Horizon::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Horizon::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
		Horizon::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	


	ImGui::End();
}

void Sandbox2D::OnEvent(Horizon::Event& e)		
{
	m_CameraController.OnEvent(e);
}