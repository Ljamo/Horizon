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
        const uint32_t MaxMeshVertices = 100000;
        const uint32_t MaxMeshIndices = 150000;
        static const uint32_t MaxTextureSlots = 32;

        Ref<VertexArray> MeshVertexArray;
        Ref<VertexBuffer> MeshVertexBuffer;
        Ref<IndexBuffer> MeshIndexBuffer;

        Vertex* MeshVertexBufferBase = nullptr;
        Vertex* MeshVertexBufferPtr = nullptr;
        uint32_t MeshIndexCount = 0;
        uint32_t* MeshIndexBufferBase = nullptr;

        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = White Texture
    };

    static Renderer3DData s_V3Data;

    void Renderer3D::Init()
    {
        s_V3Data.MeshVertexArray = VertexArray::Create();

        s_V3Data.MeshVertexBuffer = VertexBuffer::Create(s_V3Data.MaxMeshVertices * sizeof(Vertex));
        s_V3Data.MeshVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal"   },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Float,  "a_TexIndex" }
        });
        s_V3Data.MeshVertexArray->AddVertexBuffer(s_V3Data.MeshVertexBuffer);

        s_V3Data.MeshVertexBufferBase = new Vertex[s_V3Data.MaxMeshVertices];
        s_V3Data.MeshVertexBufferPtr = s_V3Data.MeshVertexBufferBase;

        s_V3Data.MeshIndexBufferBase = new uint32_t[s_V3Data.MaxMeshIndices];
        // s_V3Data.MeshIndexCount = 0;

        // Create an empty index buffer initially.
        s_V3Data.MeshIndexBuffer = IndexBuffer::Create(nullptr, 0);
        s_V3Data.MeshVertexArray->SetIndexBuffer(s_V3Data.MeshIndexBuffer);

        s_V3Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_V3Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_V3Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_V3Data.MaxTextureSlots; i++)
            samplers[i] = i;

        s_V3Data.TextureShader = Shader::Create("assets/shaders/Texture3D.glsl");
        s_V3Data.TextureShader->Bind();
        s_V3Data.TextureShader->SetIntArray("u_Textures", samplers, s_V3Data.MaxTextureSlots);

        s_V3Data.TextureSlots[0] = s_V3Data.WhiteTexture;
    }

    void Renderer3D::Flush()
    {
        // HZ_PROFILE_FUNCTION();
        // 
        // // Compute the size (in bytes) of the vertex data written.
        uint32_t vertexDataSize = (uint32_t)((uint8_t*)s_V3Data.MeshVertexBufferPtr - (uint8_t*)s_V3Data.MeshVertexBufferBase);
        s_V3Data.MeshVertexBuffer->SetData(s_V3Data.MeshVertexBufferBase, vertexDataSize);
        
        // Recreate the index buffer with the new data.
        s_V3Data.MeshIndexBuffer = IndexBuffer::Create(s_V3Data.MeshIndexBufferBase, s_V3Data.MeshIndexCount);
        s_V3Data.MeshVertexArray->SetIndexBuffer(s_V3Data.MeshIndexBuffer);
        // 
        // RenderCommand::DrawIndexed(s_V3Data.MeshVertexArray, s_V3Data.MeshIndexCount);

        HZ_PROFILE_FUNCTION();


        for (uint32_t i = 0; i < s_V3Data.TextureSlotIndex; i++)
        {
            s_V3Data.TextureSlots[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(s_V3Data.MeshVertexArray, s_V3Data.MeshIndexCount);
		// HZ_CORE_WARN("Renderer3D::Flush.");
    }

    void Renderer3D::DrawMesh(const Ref<Horizon::Model>& model)
    {
        HZ_PROFILE_FUNCTION();

        float textureIndex = 0.0f;

        const auto& meshes = model->GetMeshes();

        // Check if the texture is already bound
        uint32_t numTex = model->GetLoadedTextures().size();
		uint32_t foundTex = 0;
		std::vector<Ref<Texture2D>> unboundTextures;
		for (auto& texture : model->GetLoadedTextures())
        {
            bool isBound = false;
            for (uint32_t i = 1; i < s_V3Data.TextureSlotIndex; i++)
            {
                if (*s_V3Data.TextureSlots[i].get() == *texture.get())
                {
                    textureIndex = (float)i;
                    foundTex++;
                    isBound = true;
                    break;
                }
            }
            if (!isBound)
            {
                unboundTextures.push_back(texture);
            }
        }

		// If the texture is not bound, bind it
        if (!unboundTextures.empty())
        {
            for (auto& texture : unboundTextures)
            {
                s_V3Data.TextureSlots[s_V3Data.TextureSlotIndex] = texture;
                textureIndex = (float)s_V3Data.TextureSlotIndex;
                s_V3Data.TextureSlotIndex++;
            }
        }

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
                s_V3Data.MeshVertexBufferPtr->Color = vertex.Color;
                s_V3Data.MeshVertexBufferPtr->TexIndex = textureIndex;
                s_V3Data.MeshVertexBufferPtr++;
            }

            // Append indices with the correct offset.
            for (const auto& index : mesh.m_Indices)
            {
                s_V3Data.MeshIndexBufferBase[s_V3Data.MeshIndexCount++] = index + vertexOffset;
            }

            // Bind textures if needed.
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
		s_V3Data.TextureSlotIndex = 1;
    }

    void Renderer3D::BeginScene(const PerspectiveCamera& camera)
    {
        HZ_PROFILE_FUNCTION();
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

    void Renderer3D::Shutdown()
    {
        HZ_PROFILE_FUNCTION();

        delete[] s_V3Data.MeshVertexBufferBase;
        delete[] s_V3Data.MeshIndexBufferBase;
    }
}
