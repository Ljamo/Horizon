#include <Horizon.h>

class ExampleLayer : public Horizon::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Horizon::Event& event) override
	{
		HZ_TRACE("{0}", event);

	}
};

class Sandbox : public Horizon::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());	
		PushOverlay(new Horizon::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Horizon::Application* Horizon::CreateApplication()
{
	return new Sandbox();
}