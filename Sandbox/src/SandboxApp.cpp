#include <Horizon.h>

class Sandbox : public Horizon::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Horizon::Application* Horizon::CreateApplication()
{
	return new Sandbox();
}