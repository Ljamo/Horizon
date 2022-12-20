#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGl/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include "Horizon/Application.h"

namespace Horizon {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        
        enum ImGuiKey_
        {
            // Keyboard
            ImGuiKey_None = 0,
            ImGuiKey_Tab = 512,             // == ImGuiKey_NamedKey_BEGIN
            ImGuiKey_LeftArrow = GLFW_KEY_LEFT,
            ImGuiKey_RightArrow = GLFW_KEY_RIGHT,
            ImGuiKey_UpArrow = GLFW_KEY_UP,
            ImGuiKey_DownArrow = GLFW_KEY_DOWN,
            ImGuiKey_PageUp = GLFW_KEY_UP,
            ImGuiKey_PageDown = GLFW_KEY_DOWN,
            ImGuiKey_Home = GLFW_KEY_HOME,
            ImGuiKey_End = GLFW_KEY_END,
            ImGuiKey_Insert = GLFW_KEY_INSERT,
            ImGuiKey_Delete = GLFW_KEY_DELETE,
            ImGuiKey_Backspace = GLFW_KEY_BACKSPACE,
            ImGuiKey_Space = GLFW_KEY_SPACE,
            ImGuiKey_Enter = GLFW_KEY_ENTER,
            ImGuiKey_Escape = GLFW_KEY_ESCAPE,
            ImGuiKey_LeftCtrl = GLFW_KEY_LEFT_CONTROL, ImGuiKey_LeftShift, ImGuiKey_LeftAlt, ImGuiKey_LeftSuper,
            ImGuiKey_RightCtrl, ImGuiKey_RightShift, ImGuiKey_RightAlt, ImGuiKey_RightSuper,
            ImGuiKey_Menu,
            ImGuiKey_0, ImGuiKey_1, ImGuiKey_2, ImGuiKey_3, ImGuiKey_4, ImGuiKey_5, ImGuiKey_6, ImGuiKey_7, ImGuiKey_8, ImGuiKey_9,
            ImGuiKey_A = GLFW_KEY_A, ImGuiKey_B = GLFW_KEY_B, ImGuiKey_C = GLFW_KEY_C, ImGuiKey_D, ImGuiKey_E, ImGuiKey_F, ImGuiKey_G, ImGuiKey_H, ImGuiKey_I, ImGuiKey_J,
            ImGuiKey_K, ImGuiKey_L, ImGuiKey_M, ImGuiKey_N, ImGuiKey_O, ImGuiKey_P, ImGuiKey_Q, ImGuiKey_R, ImGuiKey_S, ImGuiKey_T,
            ImGuiKey_U, ImGuiKey_V = GLFW_KEY_V, ImGuiKey_W, ImGuiKey_X = GLFW_KEY_X, ImGuiKey_Y = GLFW_KEY_Y, ImGuiKey_Z = GLFW_KEY_Z,
            ImGuiKey_F1, ImGuiKey_F2, ImGuiKey_F3, ImGuiKey_F4, ImGuiKey_F5, ImGuiKey_F6,
            ImGuiKey_F7, ImGuiKey_F8, ImGuiKey_F9, ImGuiKey_F10, ImGuiKey_F11, ImGuiKey_F12,
            ImGuiKey_Apostrophe,        // '
            ImGuiKey_Comma,             // ,
            ImGuiKey_Minus,             // -
            ImGuiKey_Period,            // .
            ImGuiKey_Slash,             // /
            ImGuiKey_Semicolon,         // ;
            ImGuiKey_Equal,             // =
            ImGuiKey_LeftBracket,       // [
            ImGuiKey_Backslash,         // \ (this text inhibit multiline comment caused by backslash)
            ImGuiKey_RightBracket,      // ]
            ImGuiKey_GraveAccent,       // `
            ImGuiKey_CapsLock,
            ImGuiKey_ScrollLock,
            ImGuiKey_NumLock,
            ImGuiKey_PrintScreen,
            ImGuiKey_Pause,
            ImGuiKey_Keypad0, ImGuiKey_Keypad1, ImGuiKey_Keypad2, ImGuiKey_Keypad3, ImGuiKey_Keypad4,
            ImGuiKey_Keypad5, ImGuiKey_Keypad6, ImGuiKey_Keypad7, ImGuiKey_Keypad8, ImGuiKey_Keypad9,
            ImGuiKey_KeypadDecimal,
            ImGuiKey_KeypadDivide,
            ImGuiKey_KeypadMultiply,
            ImGuiKey_KeypadSubtract,
            ImGuiKey_KeypadAdd,
            ImGuiKey_KeypadEnter,
            ImGuiKey_KeypadEqual
        };

        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{

	}
}