#include <Horizon.h>
#include "Horizon/Core/EntryPoint.h"

#include "2D/Sandbox2D.h"
#include "3D/Sandbox3D.h"

class Sandbox : public Horizon::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox3D());
	}

	~Sandbox()
	{

	}
};

Horizon::Application* Horizon::CreateApplication()
{
	return new Sandbox();
}