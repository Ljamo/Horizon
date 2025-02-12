#include "hzpch.h"
#include "Renderer3D.h"

#include "../VertexArray.h"
#include "../Shader.h"
#include "../RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Horizon
{
    struct Renderer3DData
	{
        static const uint32_t MaxMeshVertices = 100000;
        static const uint32_t MaxMeshIndices = 150000;

        Ref<VertexArray> MeshVertexArray;
        Ref<VertexBuffer> MeshVertexBuffer;
        Ref<IndexBuffer> MeshIndexBuffer;

        // TODO This thing
        void* MeshVertexBufferBase = nullptr;
        void* MeshVertexBufferPtr = nullptr;
        uint32_t MeshIndexCount = 0;
        uint32_t* MeshIndexBufferBase = nullptr;

        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
	};

	static Renderer3DData s_V3Data;

    // struct PerVertex
    // {
    //     glm::vec3 Position;
    //     glm::vec3 TexCoord;
    //     // TODO Colour, TextureID
    // };

    void Renderer3D::Init()
    {
        // Create Vertex Array;

        // Create dynamic vertex buffer.
		s_V3Data.MeshVertexBuffer = VertexBuffer::Create(Renderer3DData::MaxMeshVertices * sizeof(int)); // Should be sizeof(PerVertex)
        s_V3Data.MeshVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal"   },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float4, "a_Color"    }
            });
        //s_V3Data.MeshVertexArray->AddVertexBuffer(s_V3Data.MeshVertexBuffer);
		// Add vertex Array to vertex buffer

		// Allocate CPU memory for vertices.
        // s_V3Data.MeshVertexBufferBase = new MeshVertex[Renderer3DData::MaxMeshVertices];
        //s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;

        // Allocate CPU memory for indices.
        s_V3Data.MeshIndexBufferBase = new uint32_t[Renderer3DData::MaxMeshIndices];
        s_V3Data.MeshIndexCount = 0;

        // Create a dummy index buffer; it will be recreated in EndScene.
        s_V3Data.MeshIndexBuffer = IndexBuffer::Create(nullptr, 0);

        // Setup default white texture.
        s_V3Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_V3Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        // Setup shader.
        s_V3Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer3D::Flush()
    {
        HZ_PROFILE_FUNCTION();

        // Update vertex buffer.
        uint32_t vertexDataSize = (uint32_t)((uint8_t*)s_V3Data.MeshVertexBufferPtr - (uint8_t*)s_V3Data.MeshVertexBufferBase);
        s_V3Data.MeshVertexBuffer->SetData(s_V3Data.MeshVertexBufferBase, vertexDataSize);

        // Recreate the index buffer with current batched indices.
        s_V3Data.MeshIndexBuffer = IndexBuffer::Create(s_V3Data.MeshIndexBufferBase, s_V3Data.MeshIndexCount);
        s_V3Data.MeshVertexArray->SetIndexBuffer(s_V3Data.MeshIndexBuffer);

        RenderCommand::DrawIndexed(s_V3Data.MeshVertexArray, s_V3Data.MeshIndexCount);
    }

    void Renderer3D::DrawMesh(const Model& model, const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const glm::vec4& color)
    {
		HZ_PROFILE_FUNCTION();



    }

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        s_V3Data.MeshIndexCount = 0;
        s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		HZ_PROFILE_FUNCTION();
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        s_V3Data.MeshIndexCount = 0;
        s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;
	}

	void Renderer3D::Shutdown()
	{
        HZ_PROFILE_FUNCTION();

        // delete[] s_V3Data.MeshVertexBufferBase;
        delete[] s_V3Data.MeshIndexBufferBase;
	}

	void Renderer3D::EndScene()
	{
        Flush();
	}
}
