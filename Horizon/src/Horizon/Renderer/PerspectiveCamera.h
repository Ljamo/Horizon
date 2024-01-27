#pragma once

#include <glm/glm.hpp>

namespace Horizon
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov, float AspectRatio, float NearClip, float FarClip);

		void SetProjection(float fov, float AspectRatio, float NearClip, float FarClip);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::vec3& GetFront() const { return m_Front; }
		void SetFront(const glm::vec3& front) { m_Front = glm::normalize(front); RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
	};
}