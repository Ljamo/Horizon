#include "hzpch.h"
#include "Renderer3D.h"

#include "../VertexArray.h"
#include "../Shader.h"
#include "../RenderCommand.h"
#include "../Model/Model.h"
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

        Vertex* MeshVertexBufferBase = nullptr;
        Vertex* MeshVertexBufferPtr = nullptr;
        uint32_t MeshIndexCount = 0;
        uint32_t* MeshIndexBufferBase = nullptr;

        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer3DData s_V3Data;

    void Renderer3D::Init()
    {
        s_V3Data.MeshVertexArray = VertexArray::Create();

        s_V3Data.MeshVertexBuffer = VertexBuffer::Create(Renderer3DData::MaxMeshVertices * sizeof(Vertex));
        s_V3Data.MeshVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal"   },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float4, "a_Color"    }
        });
        s_V3Data.MeshVertexArray->AddVertexBuffer(s_V3Data.MeshVertexBuffer);

        s_V3Data.MeshVertexBufferBase = new Vertex[Renderer3DData::MaxMeshVertices];
        s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;

        s_V3Data.MeshIndexBufferBase = new uint32_t[Renderer3DData::MaxMeshIndices];
        s_V3Data.MeshIndexCount = 0;

        // Create an empty index buffer initially.
        s_V3Data.MeshIndexBuffer = IndexBuffer::Create(nullptr, 0);
        s_V3Data.MeshVertexArray->SetIndexBuffer(s_V3Data.MeshIndexBuffer);

        s_V3Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_V3Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_V3Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer3D::Flush()
    {
        HZ_PROFILE_FUNCTION();

        // Compute the size (in bytes) of the vertex data written.
        uint32_t vertexDataSize = (uint32_t)((uint8_t*)s_V3Data.MeshVertexBufferPtr - (uint8_t*)s_V3Data.MeshVertexBufferBase);
        s_V3Data.MeshVertexBuffer->SetData(s_V3Data.MeshVertexBufferBase, vertexDataSize);

        // Recreate the index buffer with the new data.
        s_V3Data.MeshIndexBuffer = IndexBuffer::Create(s_V3Data.MeshIndexBufferBase, s_V3Data.MeshIndexCount);
        s_V3Data.MeshVertexArray->SetIndexBuffer(s_V3Data.MeshIndexBuffer);

        RenderCommand::DrawIndexed(s_V3Data.MeshVertexArray, s_V3Data.MeshIndexCount);
    }

    void Renderer3D::DrawMesh(const Ref<Horizon::Model>& model, const glm::vec3 position)
    {
        HZ_PROFILE_FUNCTION();

        // For this example, using fixed transform values.
        // const glm::vec3 position(0.0f);
        const glm::vec3 size(1.0f);
        const glm::vec3 rotation(0.0f);
        const glm::vec4 color(1.0f);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0))
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0))
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1))
            * glm::scale(glm::mat4(1.0f), size);

        const auto& meshes = model->GetMeshes();
        for (const auto& mesh : meshes)
        {
            // Append vertices.
            for (const auto& vertex : mesh.m_Vertices)
            {
                s_V3Data.MeshVertexBufferPtr->Position = transform * glm::vec4(vertex.Position, 1.0f);
                s_V3Data.MeshVertexBufferPtr->Normal = vertex.Normal;
                s_V3Data.MeshVertexBufferPtr->TexCoord = vertex.TexCoord;
                s_V3Data.MeshVertexBufferPtr->Color = color;
                s_V3Data.MeshVertexBufferPtr++;
            }

            // Append indices.
            for (const auto& index : mesh.m_Indices)
            {
                s_V3Data.MeshIndexBufferBase[s_V3Data.MeshIndexCount++] = index;
            }

            // Bind textures if needed (for now, assuming one texture per mesh).
            for (const auto& texture : mesh.m_Textures)
            {
                texture->Bind();
                s_V3Data.TextureShader->SetInt("u_Texture", 0);
            }
        }
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

    void Renderer3D::EndScene()
    {
        Flush();
    }

    void Renderer3D::Shutdown()
    {
        HZ_PROFILE_FUNCTION();

        delete[] s_V3Data.MeshVertexBufferBase;
        delete[] s_V3Data.MeshIndexBufferBase;
    }
}
