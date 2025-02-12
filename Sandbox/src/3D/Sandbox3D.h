#pragma once

#include "Horizon.h"

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
	Horizon::Ref<Horizon::Texture2D> m_CheckerboardTexture;


	Horizon::Ref<Horizon::VertexArray> m_CubeVA;
	Horizon::Ref<Horizon::Shader> m_FlatColorShader;

	inline static Horizon::MeshVertex m_PyramidVertices[5] = {
		 { { 0.0f, 1.0f, 0.0f },    { 0, 1, 0 },    { 0.5f, 1.0f }, { 1,1,1,1 } },
		 { { -1.0f, 0.0f, -1.0f },  { 0, -1, 0 },   { 0, 0 },      { 1,1,1,1 } },
		 { {  1.0f, 0.0f, -1.0f },  { 0, -1, 0 },   { 1, 0 },      { 1,1,1,1 } },
		 { {  1.0f, 0.0f,  1.0f },  { 0, -1, 0 },   { 1, 1 },      { 1,1,1,1 } },
		 { { -1.0f, 0.0f,  1.0f },  { 0, -1, 0 },   { 0, 1 },      { 1,1,1,1 } },
	};

	inline static uint32_t m_PyramidIndices[6 * 3] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		1, 2, 3,
		3, 4, 1
	};


	// struct ProfileResult
	// {
	// 	const char* Name;
	// 	float Time;
	// };
	// 
	// std::vector<ProfileResult> m_ProfileResults;


	glm::vec4 m_CubeColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
#endif