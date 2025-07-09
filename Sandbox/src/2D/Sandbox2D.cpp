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
	Horizon::Renderer2D::ResetStats();
	{
		PROFILE_SCOPE("Renderer Prep");
		Horizon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Horizon::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 20.0f;
		PROFILE_SCOPE("Renderer Draw");
		Horizon::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Horizon::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, {0.8f, 0.2f, 0.3f, 1.0f});
		Horizon::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, {0.8f, 0.2f, 0.3f, 1.0f});
		Horizon::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Horizon::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Horizon::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
		// Horizon::Renderer2D::EndScene();

		// Horizon::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y <= 5.0f; y ++)
		{
			for (float x = -5.0f; x <= 5.0f; x ++)
			{	
				float bx = (x + 5.0f) / 10;
				float by = (y + 5.0f) / 10;
				Horizon::Renderer2D::DrawQuad({ x, y, 0.0f }, { 0.8f, 0.8f }, { bx, by, bx * by, 0.5f});
			}
		}
		
		Horizon::Renderer2D::EndScene();
	}

	m_LastFrameTime = 0.0f;
	m_LastFrameTime += ts * 1.0f;
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));


	auto stats = Horizon::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertices Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices Count: %d", stats.GetTotalIndexCount());
	ImGui::Text("FPS: %s", std::to_string(1/m_LastFrameTime).c_str());


	ImGui::End();
}

void Sandbox2D::OnEvent(Horizon::Event& e)		
{
	m_CameraController.OnEvent(e);
}