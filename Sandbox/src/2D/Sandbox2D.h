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

	Horizon::Ref<Horizon::VertexArray> m_SquareVA;
	Horizon::Ref<Horizon::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};