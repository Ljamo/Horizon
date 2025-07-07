#pragma once

#include <Horizon/Renderer/Model/Model.h>
#include <Horizon/Audio/AudioData.h>

struct Ray
{
	glm::vec3 position;
	glm::vec3 direction;
	float distance = FLT_MAX;
	Horizon::AudioData audioData;
};


class AudioTracer
{
public:
	// AudioTracer();
	void SetParameters(std::vector<Horizon::Ref<Horizon::Model>> models, const int numRays, const int maxRayBounces);

	void OnUpdate(std::vector<Horizon::Ref<Horizon::Model>> models, const glm::vec3& pos, const glm::vec3& rotation);
	void OnUpdate(const glm::vec3& pos, const glm::vec3& rotation);

private:
	void TraceRays();
	Ray TraceAudioRay(const glm::vec3& origin, const glm::vec3& direction);
	bool CheckTriHit(const glm::vec3& origin, const glm::vec3& dir, const Horizon::Mesh& tri, glm::vec3& hitPos);
	glm::vec3 CalculateNormalFromVerts(const Horizon::Mesh& mesh);
private:
	int m_NumRays = 100;
	int m_MaxRayBounces = 5;
	glm::vec3 m_PlayerPos;
	glm::vec3 m_PlayerRotation;

	std::vector<Horizon::Ref<Horizon::Model>> m_Models;
};

