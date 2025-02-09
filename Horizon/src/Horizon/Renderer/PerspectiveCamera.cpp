#include "hzpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Horizon {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane,
		glm::vec3 pos, glm::vec3 rotation, glm::vec3 up, glm::vec3 front)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane),
		m_Position(pos), m_WorldUp(up), m_Front(glm::normalize(front)), m_Rotation(rotation) 
	{
		UpdateDirectionVectors();    // Ensure direction vectors align with the rotation
		RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane) 
	{
		m_FOV = fov;
		m_AspectRatio = aspectRatio;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		RecalculateProjectionMatrix();
	}

	const glm::vec3& PerspectiveCamera::GetPosition() const {
		return m_Position;
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position) {
		m_Position = position;
		RecalculateViewMatrix();
	}

	glm::vec3 PerspectiveCamera::GetRotation() const {
		return m_Rotation;
	}

	void PerspectiveCamera::SetRotation(const glm::vec3& rotation) {
		m_Rotation = rotation;
		UpdateDirectionVectors();
		RecalculateViewMatrix();
	}

	const glm::mat4& PerspectiveCamera::GetProjectionMatrix() const {
		return m_ProjectionMatrix;
	}

	const glm::mat4& PerspectiveCamera::GetViewMatrix() const {
		return m_ViewMatrix;
	}

	const glm::mat4& PerspectiveCamera::GetViewProjectionMatrix() const {
		return m_ViewProjectionMatrix;
	}

	glm::vec3 PerspectiveCamera::GetFront() const {
		return m_Front;
	}

	glm::vec3 PerspectiveCamera::GetRight() const {
		return glm::normalize(glm::cross(m_Front, m_WorldUp));
	}

	glm::vec3 PerspectiveCamera::GetUp() const {
		return glm::normalize(glm::cross(GetRight(), m_Front));
	}

	glm::vec3 PerspectiveCamera::GetWorldUp() const {
		return m_WorldUp;
	}


	void PerspectiveCamera::RecalculateViewMatrix() 
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::UpdateDirectionVectors() 
	{
		float yaw = glm::radians(m_Rotation.y);
		float pitch = glm::radians(m_Rotation.x);

		glm::vec3 front;
		front.x = cos(yaw) * cos(pitch);
		front.y = sin(pitch);
		front.z = sin(yaw) * cos(pitch);
		m_Front = glm::normalize(front);

		// Recalculate Right and Up vectors
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}


	void PerspectiveCamera::RecalculateProjectionMatrix() 
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	
}