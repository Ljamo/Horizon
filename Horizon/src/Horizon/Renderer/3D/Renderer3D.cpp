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
	/* Model.h contains vertex struct*/

	struct Renderer3DData
	{
		//const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = 100000;
		const uint32_t MaxIndices = 150000;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> MeshVertexArray;
		Ref<VertexBuffer> MeshVertexBuffer;
		Ref<Shader> TextureShader;
		// White texture will be default
		Ref<Texture2D> WhiteTexture;

		Vertex* MeshVertexBufferBase = nullptr;
		Vertex* MeshVertexBufferPtr = nullptr;
		uint32_t MeshIndexCount = 0;
		uint32_t* MeshIndexBufferBase = nullptr;

		std::map<std::string, uint32_t> PreLoadedTextureID;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = White Texture

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
		delete s_V3Data.MeshVertexBufferBase;
		delete s_V3Data.MeshIndexBufferBase;
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		s_V3Data.TextureShader->Bind();
		s_V3Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_V3Data.MeshIndexCount = 0;

		s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;

		s_V3Data.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		s_V3Data.TextureShader->Bind();
		s_V3Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

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

		Ref<IndexBuffer> triIB = IndexBuffer::Create(s_V3Data.MeshIndexBufferBase, s_V3Data.MaxIndices);
		s_V3Data.MeshVertexArray->SetIndexBuffer(triIB);

		for (uint32_t i = 0; i < s_V3Data.TextureSlotIndex; i++)
		{
			s_V3Data.TextureSlots[i]->Bind(i);
		}
		// Draw Call
		RenderCommand::DrawIndexed(s_V3Data.MeshVertexArray, s_V3Data.MeshIndexCount);
	}

	void Renderer3D::DrawMesh(const Ref<Horizon::Model>& model)
	{
		HZ_PROFILE_FUNCTION();

		float texIndex = 0.0f;
		
		const auto& meshes = model->GetMeshes();
		const auto& textures = model->GetLoadedTextures();

		/////////////  Begin textures  //////////////////////

		// Check to see if texture is already bound
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

		/////////////  End textures  ////////////////////////

		for (const auto& mesh : meshes)
		{
			// Calculate current vertex offset.
			uint32_t vertexOffset = s_V3Data.MeshVertexBufferPtr - s_V3Data.MeshVertexBufferBase;

			// Append vertices.
			for (const auto& vertex : mesh.m_Vertices)
			{
				s_V3Data.MeshVertexBufferPtr->Position = model->GetModelMatrix() * glm::vec4(vertex.Position, 1.0f);
				s_V3Data.MeshVertexBufferPtr->Normal = vertex.Normal;
				s_V3Data.MeshVertexBufferPtr->TexCoord = vertex.TexCoord;
				// s_V3Data.MeshVertexBufferPtr->Color = vertex.Color;
				s_V3Data.MeshVertexBufferPtr->TexIndex = texIndex;
				s_V3Data.MeshVertexBufferPtr++;
			}

			// Append indices with the correct offset.
			for (const auto& index : mesh.m_Indices)
			{
				s_V3Data.MeshIndexBufferBase[s_V3Data.MeshIndexCount++] = index + vertexOffset;
			}
		}
		
	}
}
