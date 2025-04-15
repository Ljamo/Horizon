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
	std::string path3 = "assets/models/teapot.fbx";
	std::string path = "assets/models/test/Torus.fbx";
	std::string path2 = "assets/models/Ico.fbx";
	// Horizon::Model m_BaseModel(path);
	m_BaseModel = Horizon::CreateRef<Horizon::Model>(path);
	m_BaseModel2 = Horizon::CreateRef<Horizon::Model>(path2);
	m_BaseModel3 = Horizon::CreateRef<Horizon::Model>(path3);
	m_BaseModel2->SetPosition(glm::vec3(2.0f));
	//m_BaseModel3->SetPosition(glm::vec3(-2.0f));
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

	// float rotation = 100 * sin(glfwGetTime()) * ts;
	// float position = sin(glfwGetTime()) * ts;
	// 
	// m_BaseModel->Rotate(glm::vec3(rotation));
	// m_BaseModel2->Translate(glm::vec3(position));

	{
		Horizon::Renderer3D::BeginScene(m_CameraController.GetCamera());
		{
			PROFILE_SCOPE("Sandbox3D::DrawMesh");
			Horizon::Renderer3D::DrawMesh(m_BaseModel);
			Horizon::Renderer3D::DrawMesh(m_BaseModel2);
			Horizon::Renderer3D::DrawMesh(m_BaseModel3);
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