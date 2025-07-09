#pragma once

#include "Horizon.h"

#define test
#ifdef test
class Sandbox3D : public Horizon::Layer
{
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Horizon::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Horizon::Event& e) override;
private:
	Horizon::PerspectiveCameraController m_CameraController;
	// Horizon::Ref<Horizon::Texture2D> m_CheckerboardTexture;
	// Horizon::Ref<Horizon::Object> m_BaseModel;
	// Horizon::Ref<Horizon::Object> m_BaseModel2;
	// Horizon::Ref<Horizon::Object> m_BaseModel3;
	Horizon::Ref<Horizon::Object> m_BaseModel4;
	Horizon::Ref<Horizon::Object> m_Sponza;
	glm::vec3 m_Rotation = glm::vec3(-90.0f, 180.0f, 0.0f);
	glm::vec3 m_DragonPosition = glm::vec3(35.0f, 6.6f, 7.0f);
	glm::vec3 m_DragonRotation = glm::vec3(-90.0f, 180.0f, 0.0f);
	glm::vec3 m_DragonScale = glm::vec3(0.07f);
	//Horizon::Model m_BaseModel;

	Horizon::Ref<Horizon::VertexArray> m_CubeVA;
	Horizon::Ref<Horizon::Shader> m_FlatColorShader;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};
	
	std::vector<ProfileResult> m_ProfileResults;


};
#endif