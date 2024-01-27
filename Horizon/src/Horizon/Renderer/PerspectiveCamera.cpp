#include "hzpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Horizon
{
	PerspectiveCamera::PerspectiveCamera(float fov, float AspectRatio, float NearClip, float FarClip)
	{
		SetProjection(fov, AspectRatio, NearClip, FarClip);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetProjection(float fov, float AspectRatio, float NearClip, float FarClip)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), AspectRatio, NearClip, FarClip);
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::lookAt(m_Position, m_Position + m_Front, glm::vec3(0.0f, 1.0f, 0.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}