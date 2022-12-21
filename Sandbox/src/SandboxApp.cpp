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
		if (Horizon::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(Horizon::Event& event) override
	{
		if (event.GetEventType() == Horizon::EventType::KeyPressed)
		{
			Horizon::KeyPressedEvent& e = (Horizon::KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_TAB)
				HZ_TRACE("Tab key is pressed (event)!");
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
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