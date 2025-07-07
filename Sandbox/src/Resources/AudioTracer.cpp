#include "AudioTracer.h"


void AudioTracer::SetParameters(std::vector<Horizon::Ref<Horizon::Model>> models, const int numRays = 100, const int maxRayBounces = 5)
{
	m_Models = models;
	m_NumRays = numRays;
	m_MaxRayBounces = maxRayBounces;
}

void AudioTracer::OnUpdate(std::vector<Horizon::Ref<Horizon::Model>> models, const glm::vec3& pos, const glm::vec3& rotation)
{
	m_Models = models;
	m_PlayerPos = pos;
	m_PlayerRotation = rotation;
	TraceRays();
}

void AudioTracer::OnUpdate(const glm::vec3& pos, const glm::vec3& rotation)
{
	m_PlayerPos = pos;
	m_PlayerRotation = rotation;
	TraceRays();
}

void AudioTracer::TraceRays()
{
	// Temporarily Algorithm
	// Check each mesh, althrough is very inefficient
	
	// We will split the rays into hemispheres, for left and right ears
	// Unsure as to whether or not the rays will be distributed evenly or with a weight bias


}

Ray AudioTracer::TraceAudioRay(const glm::vec3& origin, const glm::vec3& direction)
{
	Ray audioRay;


	// TODO implement BVH for more efficient tracing
	for (const auto& model : m_Models)
	{
		for (const auto& mesh : model->GetMeshes())
		{
			Ray hit;
			if (CheckTriHit(origin, direction, mesh, hit.position))
			{
				float dist = glm::length(hit.position - origin);
				if (dist < audioRay.distance)
				{
					audioRay.position = hit.position;
					audioRay.distance = dist;
					audioRay.direction = glm::reflect(direction, CalculateNormalFromVerts(mesh));
					audioRay.audioData *= mesh.GetAudioProperties();
				}
			}
		}
	}

	return audioRay;
}

bool AudioTracer::CheckTriHit(const glm::vec3& origin, const glm::vec3& dir, const Horizon::Mesh& mesh, glm::vec3& hitPos)
{
	bool hit = false;
	std::vector<Horizon::Vertex> verts = mesh.m_Vertices;
	float closestHit;

	return hit;
}

glm::vec3 AudioTracer::CalculateNormalFromVerts(const Horizon::Mesh& mesh)
{
	std::vector<Horizon::Vertex> verts = mesh.m_Vertices;

	glm::vec3 edge1 = verts[1].Position - verts[0].Position;
	glm::vec3 edge2 = verts[2].Position - verts[0].Position;
	glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
	return normal;
}