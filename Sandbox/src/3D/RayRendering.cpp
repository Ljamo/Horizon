#include "RayRendering.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Timer.h"

#include <GLFW/include/GLFW/glfw3.h>
#include <Horizon/Renderer/3D/RayRenderer.cpp>

RayRendering::RayRendering()
	: Layer("RayRendering"), m_CameraController(1280.0f / 720.0f, 75.0f, true)
{
	// HZ_TRACE("Created Sandbox3D");
	HZ_SET_ALT();
}

void RayRendering::OnAttach()
{
	m_Spheres.push_back({ glm::vec3(0.0, 0.0, -3.0), 1.0f, glm::vec3(1,0,0), 0.0f });
	m_Spheres.push_back({ glm::vec3(2,1,0), 0.5f, glm::vec3(0,1,0), 0.0f });

	// Upload to GPU
	m_GPUDataBuffer->SetData(m_Spheres.data(), (uint32_t)(m_Spheres.size() * sizeof(Sphere)));
	m_GPUDataBuffer->Bind();


}

void RayRendering::OnDetach()
{

}


void RayRendering::OnUpdate(Horizon::Timestep ts)
{
	// Update
	PROFILE_SCOPE("Sandbox3D::OnUpdate");
	{
		HZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	//Horizon::RayRenderer::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Horizon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Horizon::RenderCommand::Clear();
	}


	Horizon::RayRenderer::BeginScene(m_CameraController.GetCamera(), m_App.GetDimensions());
}

void RayRendering::OnImGuiRender()
{
	HZ_PROFILE_SCOPE();
	ImGui::Begin("Settings");

	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
		if (result.Name == "Sandbox3D::OnUpdate")
		{
			float fpsValue = 1000 / result.Time;
			std::string fps = "FPS: " + std::to_string(fpsValue);
			ImGui::Text("%s", fps.c_str());
		}
	}
	m_ProfileResults.clear();

	ImGui::Text("Test");

	ImGui::End();

}

void RayRendering::OnEvent(Horizon::Event& e)
{
	m_CameraController.OnEvent(e);
};