#pragma once

#include <Horizon.h>

struct ParticleProps
{
	glm::vec3 Position;
	glm::vec3 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void Emit(const ParticleProps& particleProps);

	void OnUpdate(Horizon::Timestep ts);
	void OnRender();
private:
	struct Particle
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		glm::vec3 Rotation = glm::vec3(0.0f);
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;
};