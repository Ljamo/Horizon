#include "hzpch.h"
#include "Renderer3D.h"

#include "../VertexArray.h"
#include "../Shader.h"
#include "../RenderCommand.h"
#include "../Model/Model.h"
#include <glm/gtc/matrix_transform.hpp>

#include <map>

namespace Horizon
{

	struct Renderer3DData
	{
		static const uint32_t MaxMeshes = 20000;
		static const uint32_t VerticesPerMesh = 4;
		static const uint32_t IndicesPerMesh = 6;

		static const uint32_t MaxVertices = MaxMeshes * VerticesPerMesh;
		static const uint32_t MaxIndices = MaxMeshes * IndicesPerMesh;
		static const uint32_t MaxTextureSlots = 32;
	
		Ref<VertexArray> MeshVertexArray;
		Ref<VertexBuffer> MeshVertexBuffer;
		Ref<Shader> TextureShader;
	
		Ref<Texture2D> WhiteTexture;
		Vertex* MeshVertexBufferBase = nullptr;
		Vertex* MeshVertexBufferPtr = nullptr;
		uint32_t MeshIndexCount = 0;
		uint32_t* MeshIndexBufferBase = nullptr;
	
		std::map<std::string, uint32_t> PreLoadedTextureID;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = White Texture
	
		Renderer3D::Statistics Stats;

		glm::mat4 ViewProjection = glm::mat4(1.0f);
	};

	static Renderer3DData s_V3Data;

	std::array<Ref<Texture2D>, Renderer3DData::MaxTextureSlots>& GetTextures()
	{
		return s_V3Data.TextureSlots;
	}

	void Renderer3D::Init()
	{
		HZ_PROFILE_FUNCTION();

		s_V3Data.MeshVertexArray = VertexArray::Create();

		s_V3Data.MeshVertexBuffer = VertexBuffer::Create(s_V3Data.MaxVertices * sizeof(Vertex));
		s_V3Data.MeshVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			//{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" }
			});
		s_V3Data.MeshVertexArray->AddVertexBuffer(s_V3Data.MeshVertexBuffer);

		s_V3Data.MeshVertexBufferBase = new Vertex[s_V3Data.MaxVertices];

		// Should make sure that this works
		s_V3Data.MeshIndexBufferBase = new uint32_t[s_V3Data.MaxIndices];

		s_V3Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_V3Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_V3Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_V3Data.MaxTextureSlots; i++)
		{
			// Set each texture slot to incremented ID
			samplers[i] = i;
		}

		s_V3Data.TextureShader = Shader::Create("assets/shaders/Texture3d.glsl");
		s_V3Data.TextureShader->Bind();
		s_V3Data.TextureShader->SetIntArray("u_Textures", samplers, s_V3Data.MaxTextureSlots);

		// Set default white texture to 0 index
		s_V3Data.TextureSlots[0] = s_V3Data.WhiteTexture;
	}

	void Renderer3D::Shutdown()
	{
		delete[] s_V3Data.MeshVertexBufferBase;
		delete[] s_V3Data.MeshIndexBufferBase;
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		s_V3Data.TextureShader->Bind();
		s_V3Data.ViewProjection = camera.GetViewProjectionMatrix();
		s_V3Data.TextureShader->SetMat4("u_ViewProjection", s_V3Data.ViewProjection);

		s_V3Data.MeshIndexCount = 0;

		s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;

		s_V3Data.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		s_V3Data.TextureShader->Bind();
		s_V3Data.ViewProjection = camera.GetViewProjectionMatrix();
		s_V3Data.TextureShader->SetMat4("u_ViewProjection", s_V3Data.ViewProjection);

		s_V3Data.MeshIndexCount = 0;

		s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;

		s_V3Data.TextureSlotIndex = 1;
	}

	void Renderer3D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_V3Data.MeshVertexBufferPtr - (uint8_t*)s_V3Data.MeshVertexBufferBase;

		s_V3Data.MeshVertexBuffer->SetData(s_V3Data.MeshVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer3D::Flush()
	{
		HZ_PROFILE_FUNCTION();

		Ref<IndexBuffer> triIB = IndexBuffer::Create(s_V3Data.MeshIndexBufferBase, s_V3Data.MeshIndexCount);
		s_V3Data.MeshVertexArray->SetIndexBuffer(triIB);

		for (uint32_t i = 0; i < s_V3Data.TextureSlotIndex; i++)
		{
			s_V3Data.TextureSlots[i]->Bind(i);
		}
		// Draw Call
		RenderCommand::DrawIndexed(s_V3Data.MeshVertexArray, s_V3Data.MeshIndexCount);
		s_V3Data.Stats.DrawCalls++;
	}

	void Renderer3D::DrawObject(const Ref<Horizon::Object> object, const glm::vec3& position,
		const glm::vec3& rotation, const glm::vec3& scale)
	{
		HZ_PROFILE_FUNCTION();

		float texIndex = 0.0f;

		std::vector<Model> models = object->GetModels();
		DrawModels(models, position, rotation, scale);
		
	}

	void Renderer3D::DrawModels(const std::vector<Model>& models, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0,0,1 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0,1,0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1,0,0 })
			* glm::scale(glm::mat4(1.0f), scale);

		for (auto& model : models)
		{
			auto meshes = model.GetMeshes();
			s_V3Data.Stats.MeshCount += meshes.size();

			for (auto& mesh : meshes)
			{
				float texIndex = 0.0f;
				std::vector<Ref<Texture2D>> textures = mesh->GetTextures();

				uint32_t verticesToAdd = mesh->GetVertices().size();
				uint32_t indicesToAdd = mesh->GetIndices().size();

				// Should probably check if the mesh data is too big for standard render3ddata
				// if it is make a new struct with fitting data.

				if (verticesToAdd > Renderer3DData::MaxVertices ||
					indicesToAdd > Renderer3DData::MaxIndices)
				{
					ImmediateDrawMesh(mesh, transform);
					continue;
				}

				if ((s_V3Data.MeshVertexBufferPtr - s_V3Data.MeshVertexBufferBase) + verticesToAdd > Renderer3DData::MaxVertices ||
					s_V3Data.MeshIndexCount + indicesToAdd > Renderer3DData::MaxIndices ||
					s_V3Data.TextureSlotIndex + textures.size() > Renderer3DData::MaxTextureSlots)
				{
					FlushAndReset();
				}


				for (const auto texture : textures)
				{
					bool alreadyExists = false;
					for (uint32_t i = 1; i < s_V3Data.TextureSlotIndex; i++)
					{
						// if ((*s_V3Data.TextureSlots[i].get() == *texture.get()) || (s_V3Data.TextureSlots[i].get()->GetPath() == texture.get()->GetPath()))
						// I don't believe checking all textures for same id will work with 3d assimp imported texture
						if ((texture.get()->HasPath()) && (s_V3Data.TextureSlots[i].get()->GetPath() == texture.get()->GetPath()))
						{
							//HZ_CORE_INFO(texture.get()->GetPath());
							texIndex = (float)i;
							alreadyExists = true;
							break;
						}
					}

					// If texture is not bound, bind
					if (!alreadyExists)
					{
						texIndex = (float)s_V3Data.TextureSlotIndex;
						s_V3Data.TextureSlots[s_V3Data.TextureSlotIndex] = texture;
						s_V3Data.TextureSlotIndex++;
					}
				}


				// Calculate current vertex offset.
				uint32_t vertexOffset = s_V3Data.MeshVertexBufferPtr - s_V3Data.MeshVertexBufferBase;

				s_V3Data.Stats.VertexCount += verticesToAdd;
				// Append vertices

				std::vector<Vertex> verts = mesh->GetVertices();
				for (const auto& vertex : verts)
				{
					s_V3Data.MeshVertexBufferPtr->Position = transform * glm::vec4(vertex.Position, 1.0f);
					s_V3Data.MeshVertexBufferPtr->Normal = vertex.Normal;
					s_V3Data.MeshVertexBufferPtr->TexCoord = vertex.TexCoord;
					// s_V3Data.MeshVertexBufferPtr->Color = vertex.Color;
					s_V3Data.MeshVertexBufferPtr->TexIndex = texIndex;
					s_V3Data.MeshVertexBufferPtr++;
				}

				s_V3Data.Stats.IndexCount += mesh->GetIndices().size();
				// Append indices with the correct offset.
				std::vector<uint32_t> indices = mesh->GetIndices();
				for (const auto& index : indices)
				{
					s_V3Data.MeshIndexBufferBase[s_V3Data.MeshIndexCount++] = index + vertexOffset;
				}
			}
		}
	}

	void Renderer3D::ResetStats()
	{
		memset(&s_V3Data.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_V3Data.Stats;
	}

	void Renderer3D::FlushAndReset()
	{
		EndScene();

		s_V3Data.MeshIndexCount = 0;

		s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;

		s_V3Data.TextureSlotIndex = 1;
	}
	void Renderer3D::ImmediateDrawMesh(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform)
	{
		auto verts = mesh->GetVertices();
		auto inds = mesh->GetIndices();
		auto texs = mesh->GetTextures();

		std::vector<Vertex> cpuVerts;
		cpuVerts.reserve(verts.size());
		for (auto& v : verts)
		{
			Vertex tv = v;
			// apply model transform now
			tv.Position = (transform * glm::vec4(v.Position, 1.0f));
			cpuVerts.push_back(tv);
		}
		Ref<VertexArray> vao = VertexArray::Create();
		Ref<VertexBuffer> vbo = VertexBuffer::Create(cpuVerts.size() * sizeof(Vertex));
		vbo->SetLayout({
		  { ShaderDataType::Float3, "a_Position" },
		  { ShaderDataType::Float3, "a_Normal"   },
		  { ShaderDataType::Float2, "a_TexCoord" },
		  { ShaderDataType::Float,  "a_TexIndex" }
			});
		vao->AddVertexBuffer(vbo);
		vbo->SetData(cpuVerts.data(), cpuVerts.size() * sizeof(Vertex));

		Ref<IndexBuffer> ibo = IndexBuffer::Create(inds.data(), inds.size());
		vao->SetIndexBuffer(ibo);

		s_V3Data.TextureShader->Bind();
		s_V3Data.TextureShader->SetMat4("u_ViewProjection", s_V3Data.ViewProjection);


		s_V3Data.TextureSlots[0] = s_V3Data.WhiteTexture;
		for (uint32_t i = 0; i < texs.size(); i++)
		{
			texs[i]->Bind(i+1);
		}


		RenderCommand::DrawIndexed(vao, (uint32_t)inds.size());

		// 7) Stats, if you like
		s_V3Data.Stats.DrawCalls++;
		s_V3Data.Stats.MeshCount++;
		s_V3Data.Stats.VertexCount += (uint32_t)cpuVerts.size();
		s_V3Data.Stats.IndexCount += (uint32_t)inds.size();
	}
}