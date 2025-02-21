#include "Sandbox3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Timer.h"

#include <GLFW/include/GLFW/glfw3.h>

#define test
#ifdef test
Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f, 75.0f, true)
{
	// HZ_TRACE("Created Sandbox3D");
	HZ_SET_3D();
}

void Sandbox3D::OnAttach()
{
	// m_CheckerboardTexture = Horizon::Texture2D::Create("assets/textures/Checkerboard.png");
	std::string path = "assets/models/monkey.fbx";
	// Horizon::Model m_BaseModel(path);
	m_BaseModel = Horizon::CreateRef<Horizon::Model>(path);
	// m_BaseModel->LoadModel();
}

void Sandbox3D::OnDetach()
{

}


void Sandbox3D::OnUpdate(Horizon::Timestep ts)
{
	// Update
	PROFILE_SCOPE("Sandbox3D::OnUpdate");
	// HZ_PROFILE_SCOPE("Sandbox3D::OnUpdate");

	{
		HZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Horizon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Horizon::RenderCommand::Clear();
	}

	{

		Horizon::Renderer3D::BeginScene(m_CameraController.GetCamera());
		// Horizon::Renderer3D::DrawCube({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		// Horizon::Renderer3D::DrawCube({ 00.5f, sin(glfwGetTime()-0.1 * sin(2 * glfwGetTime()) * 2), -0.5f}, {0.5f, 0.75f, 0.5f}, {0.2f, 0.3f, 0.8f, 1.0f});
		// //Horizon::Renderer3D::DrawCube({ sin(glfwGetTime()) + 0.5f, 0.5f, -0.5f}, {0.5f, 0.75f, 0.5f}, {0.2f, 0.3f, 0.8f, 1.0f});
		// //
		// Horizon::Mesh pyramidMesh;
		// pyramidMesh.m_Vertices = {
		// 	{ { 0.0f, 1.0f, 0.0f }, {0,1,0}, {0.5f,1.0f}, {1,1,1,1} },  // Apex
		// 	{ { -1.0f, 0.0f, -1.0f }, {0,-1,0}, {0,0},      {1,1,1,1} },
		// 	{ {  1.0f, 0.0f, -1.0f }, {0,-1,0}, {1,0},      {1,1,1,1} },
		// 	{ {  1.0f, 0.0f,  1.0f }, {0,-1,0}, {1,1},      {1,1,1,1} },
		// 	{ { -1.0f, 0.0f,  1.0f }, {0,-1,0}, {0,1},      {1,1,1,1} },
		// };
		// // Define triangles for the pyramid (four side triangles)
		// pyramidMesh.Triangles = {
		// 	Horizon::Tri(0,1,2),
		// 	Horizon::Tri(0,2,3),
		// 	Horizon::Tri(0,3,4),
		// 	Horizon::Tri(0,4,1)
		// 	// Optionally, add base triangles here.
		// };
		// glm::mat4 pyramidTransform = glm::translate(glm::mat4(1.0f), { 2.0f, 0.0f, 0.0f }) *
		// 	glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f });
		// 
		// // Draw the pyramid with a distinct colour.
		// Horizon::Renderer3D::DrawMesh(pyramidMesh,
		// 	{ 2.0f,0.0f,0.0f },    // position
		// 	{ 0.5f,0.5f,0.5f },    // size
		// 	{ 0.0f,0.0f,0.0f },    // rotation
		// 	{ 1.0f,0.8f,0.2f,1.0f });
		{
			PROFILE_SCOPE("Sandbox3D::DrawMesh");
			Horizon::Renderer3D::DrawMesh(m_BaseModel);
			// Horizon::Renderer3D::DrawMesh(m_BaseModel, glm::vec3(-1.0f, -1.0f);
		}

		Horizon::Renderer3D::EndScene();
	}
}

void Sandbox3D::OnImGuiRender()
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

void Sandbox3D::OnEvent(Horizon::Event& e)
{
	m_CameraController.OnEvent(e);
};
#endif