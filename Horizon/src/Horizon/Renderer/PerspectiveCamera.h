#pragma once

#include <glm/glm.hpp>

namespace Horizon {
	class PerspectiveCamera
	{
	public:

		PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane,
			glm::vec3 pos = glm::vec3(0.0f),
			glm::vec3 rotation = glm::vec3(0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f));
		void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

		const glm::vec3& GetPosition() const;
		void SetPosition(const glm::vec3& position);

		glm::vec3 GetRotation() const;
		void SetRotation(const glm::vec3& rotation);

		// Modify this for perspective
		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewProjectionMatrix() const;

		glm::vec3 GetFront() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetUp() const;
		glm::vec3 GetWorldUp() const;

	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();
		void UpdateDirectionVectors();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	};

}