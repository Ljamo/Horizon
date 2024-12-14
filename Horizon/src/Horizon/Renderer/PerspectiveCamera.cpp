#include "hzpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Horizon {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane)
	{
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

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::translate(transform, -m_Position);

		m_ViewMatrix = transform;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
	}
}