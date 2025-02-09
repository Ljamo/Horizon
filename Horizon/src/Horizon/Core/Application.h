#pragma once

#include "Core.h"

#include "Window.h"
#include "Horizon/Core/LayerStack.h"
#include "Horizon/Events/Event.h"
#include "Horizon/Events/ApplicationEvent.h"

#include "Horizon/Core/Timestep.h"

#include "Horizon/ImGui/ImGuiLayer.h"

namespace Horizon {

	class HORIZON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

		void EndApplication() { m_Running = false; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}