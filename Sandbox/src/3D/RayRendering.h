#pragma once

#include "Horizon.h"
#include "Horizon/Renderer/Buffer.h"

class RayRendering : public Horizon::Layer
{
public:
	RayRendering();
	virtual ~RayRendering() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Horizon::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Horizon::Event& e) override;
private:
	Horizon::PerspectiveCameraController m_CameraController;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	Horizon::Ref<Horizon::StorageBuffer> m_GPUDataBuffer = Horizon::StorageBuffer::Create();
	
	struct Sphere {
		glm::vec3 center;
		float radius;
		glm::vec3 color;
		float _pad; // padding for std430 alignment
	};

	Horizon::Application& m_App = Horizon::Application::Get();


	std::vector<Sphere> m_Spheres;

};
