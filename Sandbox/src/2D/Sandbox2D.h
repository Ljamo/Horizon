#pragma once

#include "Horizon.h"

class Sandbox2D : public Horizon::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Horizon::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Horizon::Event& e) override;
private:
	Horizon::OrthographicCameraController m_CameraController;

	// Temp
	Horizon::Ref<Horizon::VertexArray> m_SquareVA;
	Horizon::Ref<Horizon::Shader> m_FlatColorShader;

	Horizon::Ref<Horizon::Texture2D> m_CheckerboardTexture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};