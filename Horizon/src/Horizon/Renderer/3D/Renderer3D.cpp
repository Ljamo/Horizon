#include "hzpch.h"
#include "Renderer3D.h"

#include "../VertexArray.h"
#include "../Shader.h"
#include "../RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Horizon
{
	struct Renderer3DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer3DStorage* s_V3Data;

    void Renderer3D::Init()
    {
        s_V3Data = new Renderer3DStorage();
        s_V3Data->QuadVertexArray = VertexArray::Create();

        // Cube vertices (positions + texture coordinates)
        float cubeVertices[5 * 24] = {
            // Positions           // TexCoords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // 0
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // 1
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // 2
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // 3
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // 4
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // 5
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // 6
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // 7

            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // 8
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // 9
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // 10
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  // 11
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // 12
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // 13
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // 14
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f   // 15
        };

        // HZ_TRACE("Created cube verts");

        // Cube indices (12 triangles, 2 per face)
        uint32_t cubeIndices[36] = {
            0, 1, 2,  2, 3, 0,  // back face
            4, 5, 6,  6, 7, 4,  // front face
            4, 0, 3,  3, 7, 4,  // left face
            1, 5, 6,  6, 2, 1,  // right face
            4, 5, 1,  1, 0, 4,  // bottom face
            3, 2, 6,  6, 7, 3   // top face
        };

        // Create Vertex Buffer and Index Buffer for Cube
        Ref<VertexBuffer> cubeVB;
        cubeVB.reset(VertexBuffer::Create(cubeVertices, sizeof(cubeVertices)));
        cubeVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
            });

        s_V3Data->QuadVertexArray->AddVertexBuffer(cubeVB);

        Ref<IndexBuffer> cubeIB;
        cubeIB.reset(IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t)));
        s_V3Data->QuadVertexArray->SetIndexBuffer(cubeIB);

        // Texture setup
        s_V3Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_V3Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        // Shader setup
        s_V3Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_V3Data->TextureShader->Bind();
        s_V3Data->TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture)
    {
        s_V3Data->TextureShader->SetFloat4("u_Color", { 0.2f, 0.3f, 0.8f, 0.5f });
        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
        s_V3Data->TextureShader->SetMat4("u_Transform", transform);
        // HZ_CORE_WARN("DrawCube Texture");

        s_V3Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_V3Data->QuadVertexArray);
    }

    void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
    {
        s_V3Data->TextureShader->SetFloat4("u_Color", color);
        s_V3Data->WhiteTexture->Bind();

        // HZ_CORE_WARN("DrawCube FlatColor");

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
        s_V3Data->TextureShader->SetMat4("u_Transform", transform);

        s_V3Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_V3Data->QuadVertexArray);
    }


	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		s_V3Data->TextureShader->Bind();
		s_V3Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		s_V3Data->TextureShader->Bind();
		s_V3Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::Shutdown()
	{
		delete s_V3Data;
	}

	void Renderer3D::EndScene()
	{
	}
}
