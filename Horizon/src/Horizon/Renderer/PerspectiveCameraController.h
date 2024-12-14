#pragma once

#include "PerspectiveCamera.h"
#include "../Core/Timestep.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/MouseEvent.h"

namespace Horizon
{
	class PerspectiveCameraController
	{
	public:
		// Constructor: Initialises with aspect ratio and optional FOV
		PerspectiveCameraController(float aspectRatio, float fov = 45.0f, bool rotation = false);

		// Updates the camera controller (movement, rotation)
		void OnUpdate(Timestep ts);

		// Handles input events (mouse, window resize)
		void OnEvent(Event& e);


		// Getters for the camera object
		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		// Getter/Setter for zoom level (if applicable)
		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; UpdateProjection(); }

	private:
		// Handles mouse scroll input for zoom
		bool OnMouseScrolled(MouseScrolledEvent& e);

		// Adjusts projection when window size changes
		bool OnWindowResized(WindowResizeEvent& e);

		// Updates the camera's projection matrix
		void UpdateProjection();

	private:
		// Camera properties
		float m_AspectRatio;                  // Aspect ratio of the view
		float m_FOV;                          // Field of view in degrees
		float m_ZoomLevel = 1.0f;             // Zoom level for scaling projection

		PerspectiveCamera m_Camera;           // Camera instance

		// Movement and rotation control
		bool m_Rotation;                      // Whether rotation is enabled
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f }; // Camera's world position
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f }; // Yaw, pitch, roll rotation

		// Movement and rotation speeds
		float m_CameraTranslationSpeed = 5.0f; // Units per second
		float m_CameraRotationSpeed = 180.0f;  // Degrees per second
	};
}
