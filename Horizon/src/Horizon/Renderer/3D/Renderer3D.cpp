#include "hzpch.h"
#include "Renderer3D.h"

#include "../VertexArray.h"
#include "../Shader.h"
#include "../RenderCommand.h"

#include "../Mesh/Model.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Horizon
{
    // Basic cube mesh data for testing.
    static MeshVertex s_CubeVertices[24] = {
        // Front face
        { { -0.5f, -0.5f,  0.5f }, { 0, 0, 1 }, { 0, 0 }, { 1,1,1,1 } },
        { {  0.5f, -0.5f,  0.5f }, { 0, 0, 1 }, { 1, 0 }, { 1,1,1,1 } },
        { {  0.5f,  0.5f,  0.5f }, { 0, 0, 1 }, { 1, 1 }, { 1,1,1,1 } },
        { { -0.5f,  0.5f,  0.5f }, { 0, 0, 1 }, { 0, 1 }, { 1,1,1,1 } },
        // Back face
        { {  0.5f, -0.5f, -0.5f }, { 0, 0, -1 }, { 0, 0 }, { 1,1,1,1 } },
        { { -0.5f, -0.5f, -0.5f }, { 0, 0, -1 }, { 1, 0 }, { 1,1,1,1 } },
        { { -0.5f,  0.5f, -0.5f }, { 0, 0, -1 }, { 1, 1 }, { 1,1,1,1 } },
        { {  0.5f,  0.5f, -0.5f }, { 0, 0, -1 }, { 0, 1 }, { 1,1,1,1 } },
        // Left face
        { { -0.5f, -0.5f, -0.5f }, { -1, 0, 0 }, { 0, 0 }, { 1,1,1,1 } },
        { { -0.5f, -0.5f,  0.5f }, { -1, 0, 0 }, { 1, 0 }, { 1,1,1,1 } },
        { { -0.5f,  0.5f,  0.5f }, { -1, 0, 0 }, { 1, 1 }, { 1,1,1,1 } },
        { { -0.5f,  0.5f, -0.5f }, { -1, 0, 0 }, { 0, 1 }, { 1,1,1,1 } },
        // Right face
        { {  0.5f, -0.5f,  0.5f }, { 1, 0, 0 }, { 0, 0 }, { 1,1,1,1 } },
        { {  0.5f, -0.5f, -0.5f }, { 1, 0, 0 }, { 1, 0 }, { 1,1,1,1 } },
        { {  0.5f,  0.5f, -0.5f }, { 1, 0, 0 }, { 1, 1 }, { 1,1,1,1 } },
        { {  0.5f,  0.5f,  0.5f }, { 1, 0, 0 }, { 0, 1 }, { 1,1,1,1 } },
        // Top face
        { { -0.5f,  0.5f,  0.5f }, { 0, 1, 0 }, { 0, 0 }, { 1,1,1,1 } },
        { {  0.5f,  0.5f,  0.5f }, { 0, 1, 0 }, { 1, 0 }, { 1,1,1,1 } },
        { {  0.5f,  0.5f, -0.5f }, { 0, 1, 0 }, { 1, 1 }, { 1,1,1,1 } },
        { { -0.5f,  0.5f, -0.5f }, { 0, 1, 0 }, { 0, 1 }, { 1,1,1,1 } },
        // Bottom face
        { { -0.5f, -0.5f, -0.5f }, { 0, -1, 0 }, { 0, 0 }, { 1,1,1,1 } },
        { {  0.5f, -0.5f, -0.5f }, { 0, -1, 0 }, { 1, 0 }, { 1,1,1,1 } },
        { {  0.5f, -0.5f,  0.5f }, { 0, -1, 0 }, { 1, 1 }, { 1,1,1,1 } },
        { { -0.5f, -0.5f,  0.5f }, { 0, -1, 0 }, { 0, 1 }, { 1,1,1,1 } }
    };

    static uint32_t s_CubeIndices[36] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };


	struct Renderer3DData
	{
        static const uint32_t MaxMeshVertices = 100000;
        static const uint32_t MaxMeshIndices = 150000;

        Ref<VertexArray> MeshVertexArray;
        Ref<VertexBuffer> MeshVertexBuffer;
        Ref<IndexBuffer> MeshIndexBuffer;

        // TODO This thing
        MeshVertex* MeshVertexBufferBase = nullptr;
        MeshVertex* MeshVertexBufferPtr = nullptr;
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
        s_V3Data.MeshVertexArray = VertexArray::Create();

        // Create dynamic vertex buffer.
        s_V3Data.MeshVertexBuffer = VertexBuffer::Create(Renderer3DData::MaxMeshVertices * sizeof(MeshVertex));
        s_V3Data.MeshVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal"   },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float4, "a_Color"    }
            });
        s_V3Data.MeshVertexArray->AddVertexBuffer(s_V3Data.MeshVertexBuffer);

        s_V3Data.MeshVertexBufferBase = new MeshVertex[Renderer3DData::MaxMeshVertices];
        s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;

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

    /*void Renderer3D::Init()
    {
        HZ_PROFILE_FUNCTION();

        s_V3Data.QuadVertexArray = VertexArray::Create();

        // Currently unneeded
        // Cube vertices (positions + texture coordinates)
        // float cubeVertices[5 * 24] = {
        //     // Positions           // TexCoords
        //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // 0
        //      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // 1
        //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // 2
        //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // 3
        //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // 4
        //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // 5
        //      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // 6
        //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // 7
        // 
        //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // 8
        //      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // 9
        //      0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // 10
        //     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  // 11
        //      0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // 12
        //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // 13
        //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // 14
        //     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f   // 15
        // };
        // 
        // // HZ_TRACE("Created cube verts");

        // Cube indices (12 triangles, 2 per face)
        // uint32_t cubeIndices[36] = {
        //     0, 1, 2,  2, 3, 0,  // back face
        //     4, 5, 6,  6, 7, 4,  // front face
        //     4, 0, 3,  3, 7, 4,  // left face
        //     1, 5, 6,  6, 2, 1,  // right face
        //     4, 5, 1,  1, 0, 4,  // bottom face
        //     3, 2, 6,  6, 7, 3   // top face
        // };

        // Create Vertex Buffer and Index Buffer for Cube
        Ref<VertexBuffer> cubeVB = VertexBuffer::Create(s_V3Data.MaxVertices * sizeof(PerVertex));

        cubeVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
            });
        s_V3Data.QuadVertexArray->AddVertexBuffer(cubeVB);

        uint32_t* cubeIndices = new uint32_t[s_V3Data.MaxIndices];

        Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, s_V3Data.MaxIndices);
        s_V3Data.QuadVertexArray->SetIndexBuffer(cubeIB);

        delete[] cubeIndices;

        // Texture setup
        s_V3Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_V3Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        // Shader setup
        s_V3Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetInt("u_Texture", 0);
    }*/

    void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), size);
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetFloat4("u_Color", color);
        s_V3Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_V3Data.WhiteTexture->Bind();
        DrawMesh(transform, s_CubeVertices, 24, s_CubeIndices, 36);
    }

    void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), size);
        s_V3Data.TextureShader->SetFloat4("u_Color", { 0.2f, 0.3f, 0.8f, 1.0f });
        s_V3Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        texture->Bind();
        DrawMesh(transform, s_CubeVertices, 24, s_CubeIndices, 36);
    }

    void Renderer3D::DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation.x, { 1.0f, 0.0f, 0.0f }) *
            glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f, 1.0f, 0.0f }) *
            glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), size);
        s_V3Data.TextureShader->SetFloat4("u_Color", color);
        s_V3Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_V3Data.WhiteTexture->Bind();
        DrawMesh(transform, s_CubeVertices, 24, s_CubeIndices, 36);
    }

    void Renderer3D::DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const Ref<Texture2D>& texture)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation.x, { 1.0f, 0.0f, 0.0f }) *
            glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f, 1.0f, 0.0f }) *
            glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), size);
        s_V3Data.TextureShader->SetFloat4("u_Color", { 0.2f, 0.3f, 0.8f, 1.0f });
        s_V3Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        texture->Bind();
        DrawMesh(transform, s_CubeVertices, 24, s_CubeIndices, 36);
    }

    void Renderer3D::DrawMesh(const glm::mat4& transform, const MeshVertex* vertices, uint32_t vertexCount,
        const uint32_t* indices, uint32_t indexCount)
    {
        // Flush if insufficient room remains.
        uint32_t currentVertexCount = (uint32_t)(s_V3Data.MeshVertexBufferPtr - s_V3Data.MeshVertexBufferBase);
        if (s_V3Data.MeshIndexCount + indexCount > Renderer3DData::MaxMeshIndices ||
            currentVertexCount + vertexCount > Renderer3DData::MaxMeshVertices)
        {
            Flush();
            s_V3Data.MeshIndexCount = 0;
            s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;
            currentVertexCount = 0;
        }

        uint32_t vertexOffset = currentVertexCount;
        // Transform and batch vertices.
        for (uint32_t i = 0; i < vertexCount; i++)
        {
            MeshVertex transformed = vertices[i];
            transformed.Position = glm::vec3(transform * glm::vec4(vertices[i].Position, 1.0f));
            *s_V3Data.MeshVertexBufferPtr++ = transformed;
        }
        // Batch indices with offset.
        for (uint32_t i = 0; i < indexCount; i++)
        {
            s_V3Data.MeshIndexBufferBase[s_V3Data.MeshIndexCount + i] = indices[i] + vertexOffset;
        }
        s_V3Data.MeshIndexCount += indexCount;
    }

    void Renderer3D::DrawMesh(const Model& model, const glm::vec3& position,
        const glm::vec3& size, const glm::vec3& rotation,
        const glm::vec4& color)
    {
        // Build transformation matrix.
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1)) *
            glm::scale(glm::mat4(1.0f), size);
        // Set shader uniforms for colour.
        s_V3Data.TextureShader->SetFloat4("u_Color", color);
        s_V3Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_V3Data.WhiteTexture->Bind();

        // Retrieve indices from the mesh.
        std::vector<uint32_t> indices = mesh.GetIndices();
        // Call the low-level DrawMesh function.
        DrawMesh(transform,
            reinterpret_cast<const MeshVertex*>(mesh.m_Vertices.data()),
            static_cast<uint32_t>(mesh.m_Vertices.size()),
            indices.data(),
            static_cast<uint32_t>(indices.size()));
    }

    // Overload: Draw a mesh with a texture.
    void Renderer3D::DrawMesh(const Model& model, const glm::vec3& position,
        const glm::vec3& size, const glm::vec3& rotation,
        const Ref<Texture2D>& texture)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1)) *
            glm::scale(glm::mat4(1.0f), size);
        // Set a default colour for texture modulation.
        s_V3Data.TextureShader->SetFloat4("u_Color", { 1.0f,1.0f,1.0f,1.0f });
        s_V3Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        texture->Bind();

        std::vector<uint32_t> indices = mesh.GetIndices();
        DrawMesh(transform,
            reinterpret_cast<const MeshVertex*>(mesh.m_Vertices.data()),
            static_cast<uint32_t>(mesh.m_Vertices.size()),
            indices.data(),
            static_cast<uint32_t>(indices.size()));
    }

   // void Renderer3D::DrawMesh(const glm::mat4& transform, const MeshVertex* vertices, const uint32_t* indices)
   // {
   //     // Flush if insufficient room remains.
   //     uint32_t currentVertexCount = (uint32_t)(s_V3Data.MeshVertexBufferPtr - s_V3Data.MeshVertexBufferBase);
   //     if (s_V3Data.MeshIndexCount + indexCount > Renderer3DData::MaxMeshIndices ||
   //         currentVertexCount + vertexCount > Renderer3DData::MaxMeshVertices)
   //     {
   //         Flush();
   //         s_V3Data.MeshIndexCount = 0;
   //         s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;
   //         currentVertexCount = 0;
   //     }
   //
   //     uint32_t vertexOffset = currentVertexCount;
   //     // Transform and batch vertices.
   //     for (uint32_t i = 0; i < vertexCount; i++)
   //     {
   //         MeshVertex transformed = vertices[i];
   //         transformed.Position = glm::vec3(transform * glm::vec4(vertices[i].Position, 1.0f));
   //         *s_V3Data.MeshVertexBufferPtr++ = transformed;
   //     }
   //     // Batch indices with offset.
   //     for (uint32_t i = 0; i < indexCount; i++)
   //     {
   //         s_V3Data.MeshIndexBufferBase[s_V3Data.MeshIndexCount + i] = indices[i] + vertexOffset;
   //     }
   //     s_V3Data.MeshIndexCount += indexCount;
   // }
    
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

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        s_V3Data.MeshIndexCount = 0;
        s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        s_V3Data.MeshIndexCount = 0;
        s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;
	}

	void Renderer3D::Shutdown()
	{
        HZ_PROFILE_FUNCTION();

        delete[] s_V3Data.MeshVertexBufferBase;
        delete[] s_V3Data.MeshIndexBufferBase;
	}

	void Renderer3D::EndScene()
	{
        Flush();
	}
}
