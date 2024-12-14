#pragma once

#include "Horizon.h"

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

	Horizon::Ref<Horizon::VertexArray> m_CubeVA;
	Horizon::Ref<Horizon::Shader> m_FlatColorShader;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;


	glm::vec4 m_CubeColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};