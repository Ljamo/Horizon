#include "Sandbox3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Timer.h"

#include <GLFW/include/GLFW/glfw3.h>
#include <Horizon/Renderer/3D/Renderer3D.cpp>

Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f, 75.0f, true)
{
	// HZ_TRACE("Created Sandbox3D");
	HZ_SET_3D();
}

void Sandbox3D::OnAttach()
{
	// m_CheckerboardTexture = Horizon::Texture2D::Create("assets/textures/Checkerboard.png");
	std::string path3 = "assets/models/Roof.fbx";
	std::string path = "assets/models/Floor.fbx";
	std::string path2 = "assets/models/Walls.fbx";
	std::string path4 = "assets/models/dragon_low.fbx";
	std::string sponza = "assets/models/sponza/sponza.obj";
	// Horizon::Model m_BaseModel(path);
	// m_BaseModel = Horizon::CreateRef<Horizon::Object>(path);
	// m_BaseModel2 = Horizon::CreateRef<Horizon::Object>(path2);
	// m_BaseModel3 = Horizon::CreateRef<Horizon::Object>(path3);
	m_BaseModel4 = Horizon::CreateRef<Horizon::Object>(path4);
	m_Sponza = Horizon::CreateRef<Horizon::Object>(sponza);

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
	Horizon::Renderer3D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Horizon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Horizon::RenderCommand::Clear();
	}

	// float rotation = 100 * sin(glfwGetTime()) * ts;
	// float position = sin(glfwGetTime()) * ts;
	// 
	// m_BaseModel->Rotate(glm::vec3(rotation));
	// m_BaseModel2->Translate(glm::vec3(position));

	// m_Rotation += glm::vec3(10.0f * ts);
	// m_BaseModel2->SetRotation(m_Rotation);

	{
		Horizon::Renderer3D::BeginScene(m_CameraController.GetCamera());
		{
			PROFILE_SCOPE("Sandbox3D::DrawMesh");
			// Horizon::Renderer3D::DrawObject(m_BaseModel, glm::vec3(0.0f), m_Rotation, glm::vec3(1.0f));
			// Horizon::Renderer3D::DrawObject(m_BaseModel2, glm::vec3(0.0f), m_Rotation, glm::vec3(1.0f));
			// Horizon::Renderer3D::DrawObject(m_BaseModel3, glm::vec3(0.0f), m_Rotation, glm::vec3(1.0f));
			Horizon::Renderer3D::DrawObject(m_BaseModel4, m_DragonPosition, m_DragonRotation, m_DragonScale);
			// Horizon::Renderer3D::DrawObject(m_Sponza, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
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

	auto stats = Horizon::Renderer3D::GetStats();
	ImGui::Text("Renderer3D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Mesh Count: %d", stats.GetTotalMeshCount());
	ImGui::Text("Vertices Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices Count: %d", stats.GetTotalIndexCount());

	// ImGui::SliderFloat3("Mesh Rotation", &m_Rotation.x, -180.0f, 180.0f);
	ImGui::SliderFloat3("Dragon Rotaton", &m_DragonRotation.x, -180.0f, 180.0f);
	ImGui::SliderFloat3("Dragon Position", &m_DragonPosition.x, -200.0f, 200.0f);
	ImGui::SliderFloat3("Dragon Scale", &m_DragonScale.x, 0.01f, 0.5f);

	// ImGui::Checkbox("Enable Backface Culling", &)

	ImGui::End();

	ImGui::Begin("Image Textures");
	auto textures = Horizon::GetTextures();
	int i = 0;
	if (textures.size() != 0)
	{
		for (auto& texture : textures)
		{
			if (texture != nullptr)
			{

			// Assuming Texture2D has a function GetRendererID() that returns GLuint
			uint32_t textureID = texture->GetRendererID();

			// Display the texture slot number as text.
			ImGui::Text("Texture Slot %d", i);

			// Display the image; adjust the ImVec2 size as needed.
			// Casting the textureID to ImTextureID:
			ImGui::Image((void*)(intptr_t)textureID, ImVec2(100, 100));
			}
			i++;
		}
	}

	ImGui::End();

}

void Sandbox3D::OnEvent(Horizon::Event& e)
{
	m_CameraController.OnEvent(e);
};