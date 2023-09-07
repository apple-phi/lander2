#pragma once
#include <vector>
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "graphics/buffer.h"

namespace Graphics
{
    struct VertexData
    {
        glm::vec3 pos;
        glm::vec2 texCoord;
    };

    struct TriangleMesh
    {
        std::vector<VertexData> vertices;
        std::vector<unsigned int> triangleIndices;

        VAO vao;
        Buffer vbo, ebo;
        TriangleMesh(const std::vector<VertexData> &vertices, const std::vector<unsigned int> &trianglesIndices);
        void setVertPosLocation(unsigned int location) const;
        void setTexCoordLocation(unsigned int location) const;
        void draw() const;
    };
    std::array<TriangleMesh, 6> generateFaces(unsigned int resolution);
    TriangleMesh makeSpherifiedCubeFace(const glm::vec3 &normal, unsigned int resolution);
}