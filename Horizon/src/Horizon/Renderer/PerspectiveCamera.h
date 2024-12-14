#pragma once

#include <glm/glm.hpp>

namespace Horizon {
	class PerspectiveCamera
	{
	public:

		// Modify for perspective
		PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);
		void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); RecalculateProjectionMatrix(); }

		glm::vec3 GetRotation() const { return m_Rotation; }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); RecalculateProjectionMatrix(); }

		// Modify this for perspective
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		glm::vec3 GetFront() const {
			return glm::normalize(glm::vec3(-m_ViewMatrix[2]));
		}
		glm::vec3 GetRight() const {
			return glm::normalize(glm::vec3(m_ViewMatrix[0]));
		}
		glm::vec3 GetUp() const {
			return glm::normalize(glm::vec3(m_ViewMatrix[1]));
		}

	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f }; // Euler angles: pitch, yaw, roll

		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;

	};

}