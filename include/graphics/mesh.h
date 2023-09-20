#pragma once
#include <vector>
#include <array>
#include <glbinding/gl/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "graphics/buffer.h"
#include "graphics/helper.h"

namespace Graphics::Meshes
{
    struct VertexData
    {
        glm::vec3 pos;
        glm::vec2 texCoord;
    };
    static_assert(sizeof(glm::vec3) == sizeof(float) * 3, "glm::vec3 is not the same size as float[3]");
    static_assert(sizeof(VertexData) == sizeof(float) * 5, "VertexData is not the same size as float[5]");

    struct TriangleMesh
    {
        unsigned int numTriangles = 0;
        VAO vao;
        Buffer vbo, ebo;
        TriangleMesh() = default;
        TriangleMesh(const std::vector<VertexData> &vertices, const std::vector<unsigned int> &trianglesIndices);
        TriangleMesh(gl::GLuint vbo, gl::GLuint ebo);
        const TriangleMesh &setVertPosLocation(unsigned int location) const;
        const TriangleMesh &setTexCoordLocation(unsigned int location) const;
        void draw() const;
    };
    std::array<TriangleMesh, 6> shaderGenerateFaces(unsigned int resolution);
}