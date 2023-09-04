#pragma once
#include <vector>
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "graphics/buffer.h"

namespace Graphics
{
    struct TriangleMesh
    {
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> triangles;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;

        VAO vao;
        Buffer vbo, ebo;
        TriangleMesh(std::vector<glm::vec3>, std::vector<unsigned int> triangles);
        TriangleMesh(std::vector<glm::vec3>, std::vector<unsigned int> triangles, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords);
        void initVAO() const;
    };
    std::array<TriangleMesh, 6> generateFaces(unsigned int resolution);
    TriangleMesh makeSpherifiedCubeFace(const glm::vec3 &normal, unsigned int resolution);
    // TriangleMesh makeSpherifiedCubeFace(unsigned int resolution, glm::vec3 normal, glm::vec2 startT, glm::vec2 endT, double radius);
}