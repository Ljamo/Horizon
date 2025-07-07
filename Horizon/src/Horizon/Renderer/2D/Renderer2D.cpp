#include "hzpch.h"
#include "Renderer2D.h"

#include "../VertexArray.h"
#include "../Shader.h"
#include "../RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Horizon
{
	struct PerVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		// TODO Colour, TextureID
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;


		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		PerVertex* QuadVertexBufferBase = nullptr;
		PerVertex* QuadVertexBufferPointer = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = White Texture

		glm::vec4 QuadVertexPositions[4];
	};

	static Renderer2DData s_V2Data;



	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		s_V2Data.QuadVertexArray = VertexArray::Create();

		s_V2Data.QuadVertexBuffer = VertexBuffer::Create(s_V2Data.MaxVertices * sizeof(PerVertex));
		s_V2Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
			});
		s_V2Data.QuadVertexArray->AddVertexBuffer(s_V2Data.QuadVertexBuffer);

		s_V2Data.QuadVertexBufferBase = new PerVertex[s_V2Data.MaxVertices];

		// Set up quad indices

		// Preset indices for quads
		uint32_t* quadIndices = new uint32_t[s_V2Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_V2Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_V2Data.MaxIndices);
		s_V2Data.QuadVertexArray->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		// End of quad index setup

		s_V2Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_V2Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_V2Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_V2Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_V2Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_V2Data.TextureShader->Bind();
		s_V2Data.TextureShader->SetIntArray("u_Textures", samplers, s_V2Data.MaxTextureSlots);

		s_V2Data.TextureSlots[0] = s_V2Data.WhiteTexture;

		s_V2Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_V2Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_V2Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_V2Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		// HZ_INFO("No V2Data to delete.");
		delete s_V2Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_V2Data.TextureShader->Bind();
		s_V2Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_V2Data.QuadIndexCount = 0;

		s_V2Data.QuadVertexBufferPointer = s_V2Data.QuadVertexBufferBase;

		s_V2Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_V2Data.QuadVertexBufferPointer - (uint8_t*)s_V2Data.QuadVertexBufferBase;
		s_V2Data.QuadVertexBuffer->SetData(s_V2Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		HZ_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_V2Data.TextureSlotIndex; i++)
		{
			s_V2Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_V2Data.QuadVertexArray, s_V2Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		const float tilingFactor = 1.0f;
		const float texIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[0];
		s_V2Data.QuadVertexBufferPointer->Color = color;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = texIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[1];
		s_V2Data.QuadVertexBufferPointer->Color = color;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = texIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[2];
		s_V2Data.QuadVertexBufferPointer->Color = color;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = texIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[3];
		s_V2Data.QuadVertexBufferPointer->Color = color;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = texIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		float textureIndex = 0.0f;

		// Check if the texture is already bound
		for (uint32_t i = 1; i < s_V2Data.TextureSlotIndex; i++)
		{
			if (*s_V2Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// If the texture is not bound, bind it
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_V2Data.TextureSlotIndex;
			s_V2Data.TextureSlots[s_V2Data.TextureSlotIndex] = texture;
			s_V2Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		// Set vertex data
		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[0];
		s_V2Data.QuadVertexBufferPointer->Color = tintColor;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[1];
		s_V2Data.QuadVertexBufferPointer->Color = tintColor;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 1.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[2];
		s_V2Data.QuadVertexBufferPointer->Color = tintColor;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 1.0f, 1.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[3];
		s_V2Data.QuadVertexBufferPointer->Color = tintColor;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 1.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadIndexCount += 6;

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		s_V2Data.TextureShader->SetFloat4("u_Color", color);
		s_V2Data.WhiteTexture->Bind();

		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		// Set vertex data
		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[0];
		s_V2Data.QuadVertexBufferPointer->Color = color;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[1];
		s_V2Data.QuadVertexBufferPointer->Color = color;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 1.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[2];
		s_V2Data.QuadVertexBufferPointer->Color = color;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 1.0f, 1.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[3];
		s_V2Data.QuadVertexBufferPointer->Color = color;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 1.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		float textureIndex = 0.0f;

		// Check if the texture is already bound
		for (uint32_t i = 1; i < s_V2Data.TextureSlotIndex; i++)
		{
			if (*s_V2Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// If the texture is not bound, bind it
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_V2Data.TextureSlotIndex;
			s_V2Data.TextureSlots[s_V2Data.TextureSlotIndex] = texture;
			s_V2Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		// Set vertex data
		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[0];
		s_V2Data.QuadVertexBufferPointer->Color = tintColor;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[1];
		s_V2Data.QuadVertexBufferPointer->Color = tintColor;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 1.0f, 0.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[2];
		s_V2Data.QuadVertexBufferPointer->Color = tintColor;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 1.0f, 1.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadVertexBufferPointer->Position = transform * s_V2Data.QuadVertexPositions[3];
		s_V2Data.QuadVertexBufferPointer->Color = tintColor;
		s_V2Data.QuadVertexBufferPointer->TexCoord = { 0.0f, 1.0f };
		s_V2Data.QuadVertexBufferPointer->TexIndex = textureIndex;
		s_V2Data.QuadVertexBufferPointer->TilingFactor = tilingFactor;
		s_V2Data.QuadVertexBufferPointer++;

		s_V2Data.QuadIndexCount += 6;

	}
}