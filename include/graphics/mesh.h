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

    struct TriangleMesh
    {
        unsigned int numTriangles;
        VAO vao;
        Buffer vbo, ebo;
        TriangleMesh();
        TriangleMesh(const std::vector<VertexData> &vertices, const std::vector<unsigned int> &trianglesIndices);
        TriangleMesh(gl::GLuint vbo, gl::GLuint ebo);
        void setVertPosLocation(unsigned int location) const;
        void setTexCoordLocation(unsigned int location) const;
        void draw() const;
    };
    std::array<TriangleMesh, 6> shaderGenerateFaces(unsigned int resolution);
}