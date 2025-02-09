#pragma once

#include "Horizon.h"

namespace Horizon
{
    // Per-vertex data.
    struct Vert
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
        glm::vec4 TintColor; // TintColour
    };

    // A simple mesh, containing vertices and triangles.
    class Mesh
    {
    public:
        Mesh(std::vector<Vert> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
            : m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
        {
            // SetupMesh();
        }

        void SetupMesh();

        const std::vector<Vert>& GetVertices() const { return m_Vertices; }
        const std::vector<uint32_t>& GetIndices() const { return m_Vertices; }
        const std::vector<Texture>& GetTextures() const { return m_Textures; }
    private:
        std::vector<Vert> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::vector<Texture> m_Textures;

    };

    // A mesh contains vertices, and indices. 
}