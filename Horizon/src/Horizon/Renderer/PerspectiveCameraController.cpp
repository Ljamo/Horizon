#include "hzpch.h"
#include "PerspectiveCameraController.h"

#include "../Core/Input.h"
#include "../Core/KeyCodes.h"

namespace Horizon {

	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, float fov, bool rotation)
		: m_AspectRatio(aspectRatio), m_FOV(fov), m_Camera(fov, aspectRatio, 0.1f, 1500.0f, m_CameraPosition, m_CameraRotation), m_Rotation(rotation)
	{
		// HZ_CORE_INFO("m_Camera should be created");
		m_Camera.SetProjection(m_FOV, m_AspectRatio, 0.1f, 1500.0f);
		// HZ_CORE_INFO("Set projection");
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		float velocity = m_CameraTranslationSpeed * ts;

		// Forward/Backward movement
		if (Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition += m_Camera.GetFront() * velocity;
		if (Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition -= m_Camera.GetFront() * velocity;

		// Left/Right movement
		if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition -= m_Camera.GetRight() * velocity;
		if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition += m_Camera.GetRight() * velocity;

		// Up/Down movement
		if (Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT))
			m_CameraPosition -= m_Camera.GetWorldUp() * velocity;
		if (Input::IsKeyPressed(HZ_KEY_SPACE))
			m_CameraPosition += m_Camera.GetWorldUp() * velocity;

		if (m_Rotation)
		{
			float rotationSpeed = m_CameraRotationSpeed * ts;

			float mouseX = Input::GetMouseX() * rotationSpeed;
			float mousey = Input::GetMouseY() * rotationSpeed;


			// Rotate camera using Up/Down/Left/Right keys
			if (Input::IsKeyPressed(HZ_KEY_UP))
				m_CameraRotation.x += rotationSpeed; // Pitch
			if (Input::IsKeyPressed(HZ_KEY_DOWN))
				m_CameraRotation.x -= rotationSpeed; // Pitch
			if (Input::IsKeyPressed(HZ_KEY_LEFT))
				m_CameraRotation.y -= rotationSpeed; // Yaw
			if (Input::IsKeyPressed(HZ_KEY_RIGHT))
				m_CameraRotation.y += rotationSpeed; // Yaw

			// Clamp pitch to prevent flipping
			m_CameraRotation.x = glm::clamp(m_CameraRotation.x, -89.0f, 89.0f);

		}

		if (Input::IsMouseButtonPressed(1))
			HZ_CORE_WARN("Mouse Pressed");

		

		// Update camera with new position and rotation
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		// m_FOV -= e.GetYOffset() * 1.0f; // Adjust sensitivity as needed
		// m_FOV = glm::clamp(m_FOV, 10.0f, 90.0f); // Clamp FOV between reasonable values
		// m_Camera.SetProjection(m_FOV, m_AspectRatio, 0.1f, 100.0f);
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(m_FOV, m_AspectRatio, 0.1f, 1500.0f);
		return false;
	}

}
