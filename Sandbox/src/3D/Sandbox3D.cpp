#include "Sandbox3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Timer.h"

Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f, 45.0f)
{
	HZ_WARN(m_CameraController.GetZoomLevel());
	HZ_TRACE("Created Sandbox3D");
}

void Sandbox3D::OnAttach()
{
	
};

void Sandbox3D::OnDetach()
{

};


void Sandbox3D::OnUpdate(Horizon::Timestep ts)
{
	// Update
	PROFILE_SCOPE("Sandbox3D::OnUpdate");

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

		Horizon::Renderer3D::BeginScene(m_CameraController.GetCamera());
		Horizon::Renderer3D::DrawCube({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Horizon::Renderer3D::DrawCube({ 0.5f, -0.5f, -0.5f }, { 0.5f, 0.75f, 0.5f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		// Horizon::Renderer3D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Horizon::Renderer3D::EndScene();
	}
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	// ImGui::ColorEdit4("Cube Color", glm::value_ptr(m_SquareColor));

	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox3D::OnEvent(Horizon::Event& e)
{
	m_CameraController.OnEvent(e);
};
