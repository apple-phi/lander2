#pragma once
#include <vector>
#include <array>
#include <glbinding/gl/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "graphics/buffer.h"

namespace Direction
{
    extern const glm::vec3 UP;
    extern const glm::vec3 DOWN;
    extern const glm::vec3 LEFT;
    extern const glm::vec3 RIGHT;
    extern const glm::vec3 FRONT;
    extern const glm::vec3 BACK;
}

namespace Graphics
{
    struct VertexData
    {
        glm::vec3 pos;
        glm::vec2 texCoord;
    };

    struct TriangleMesh
    {
        unsigned int numTriangles;
        VAO vao;
        Buffer vbo, ebo;
        TriangleMesh(const std::vector<VertexData> &vertices, const std::vector<unsigned int> &trianglesIndices);
        TriangleMesh(gl::GLuint vbo, gl::GLuint ebo);
        void setVertPosLocation(unsigned int location) const;
        void setTexCoordLocation(unsigned int location) const;
        void draw() const;
    };
    std::array<TriangleMesh, 6> generateFaces(unsigned int resolution);
    std::array<TriangleMesh, 6> shaderGenerateFaces(unsigned int resolution);
    TriangleMesh shaderMakeSpherifiedCubeFace(const glm::vec3 &normal, unsigned int resolution);
    TriangleMesh makeSpherifiedCubeFace(const glm::vec3 &normal, unsigned int resolution);
}