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
	Horizon::Ref<Horizon::Model> m_BaseModel;
	Horizon::Ref<Horizon::Model> m_BaseModel2;
	Horizon::Ref<Horizon::Model> m_BaseModel3;
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