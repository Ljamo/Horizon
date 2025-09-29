#include "hzpch.h"
#include "RayTracingCamera.h"

#include "../Core/Input.h"
#include "../Core/KeyCodes.h"
#include "../Core/MouseButtonCodes.h"

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Horizon
{

	RayTracingCamera::RayTracingCamera(float aspectRatio, float fov)
		: m_AspectRatio(aspectRatio), m_FOV(fov), m_Camera(fov, aspectRatio, 0.1f, 1500.0f, m_CameraPosition, m_CameraRotation)
	{
		// HZ_CORE_INFO("m_Camera should be created");
		m_Camera.SetProjection(m_FOV, m_AspectRatio, 0.1f, 1500.0f);
		// HZ_CORE_INFO("Set projection");
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}
	
	
	void RayTracingCamera::OnUpdate(Timestep ts)
	{
		glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 delta = (mousePos - m_LastMousePos) * 0.002f;
		m_LastMousePos = mousePos;
		
		if (!Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
		{
			Input::SetCursorMode(HZ_CURSOR_MODE_NORMAL);
			return;
		}
	
		Input::SetCursorMode(HZ_CURSOR_MODE_DISABLED);
	
		bool moved = false;
		float velocity = m_CameraTranslationSpeed * ts;

		// Forward/Backward movement
		if (Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition += m_Camera.GetFront() * velocity;
			moved = true;
		}
		if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition -= m_Camera.GetFront() * velocity;
			moved = true;
		}

		// Left/Right movement
		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition -= m_Camera.GetRight() * velocity;
			moved = true;
		}
		if (Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraPosition += m_Camera.GetRight() * velocity;
			moved = true;
		}

		// Up/Down movement
		if (Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT))
		{
			m_CameraPosition -= m_Camera.GetWorldUp() * velocity;
			moved = true;
		}
		if (Input::IsKeyPressed(HZ_KEY_SPACE))
		{
			m_CameraPosition += m_Camera.GetWorldUp() * velocity;
			moved = true;
		}
	
		if (delta.x != 0.0f || delta.y != 0.0f)
		{
			float pitchDelta = delta.y * m_CameraRotationSpeed;
			float yawDelta = delta.x * m_CameraRotationSpeed;

			glm::vec3 rot = m_Camera.GetRotation();

			rot.x -= pitchDelta; // pitch
			rot.y += yawDelta;   // yaw

			// Clamp pitch to avoid flipping upside down
			rot.x = glm::clamp(rot.x, -89.0f, 89.0f);

			// Set updated rotation
			m_Camera.SetRotation(rot);
			m_CameraRotation = rot;

			moved = true;
		}

		// Update camera with new position and rotation
		if (moved)
		{
			m_Camera.SetPosition(m_CameraPosition);
			m_Camera.SetRotation(m_CameraRotation);
		}
	}
	
	void RayTracingCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(RayTracingCamera::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(RayTracingCamera::OnWindowResized));
	}
	
	bool RayTracingCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		return false;
	}
	
	bool RayTracingCamera::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = ((float)e.GetHeight() > 0) ? (float)e.GetWidth() / (float)e.GetHeight() : 0;
		m_Camera.SetProjection(m_FOV, m_AspectRatio, 0.1f, 1500.0f);
		return false;
	}
	
}