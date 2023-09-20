#include <iostream>
#include <algorithm>
#include <glbinding/gl/gl.h>
#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/buffer.h"
#include "graphics/texture.h"
#include "graphics/helper.h"

namespace Graphics::Meshes
{
    TriangleMesh::TriangleMesh(gl::GLuint vbo, gl::GLuint ebo)
        : vbo(vbo), ebo(ebo)
    {
        if (Buffer(vbo).getSize() % sizeof(VertexData) != 0)
        {
            throw std::runtime_error("vbo.getSize() is not a multiple of sizeof(VertexData)");
        }
        if (Buffer(ebo).getSize() % (3 * sizeof(unsigned int)) != 0)
        {
            throw std::runtime_error("ebo.getSize() is not a multiple of 3 * sizeof(unsigned int)");
        }
        this->numTriangles = Buffer(ebo).getSize() / sizeof(unsigned int) / 3;
        vao
            .attachVBO(0, vbo, 0, sizeof(VertexData))
            .attachEBO(ebo);
    }
    TriangleMesh::TriangleMesh(const std::vector<VertexData> &vertices, const std::vector<unsigned int> &triangleIndices)
    {
        if (triangleIndices.size() % 3 != 0)
        {
            throw std::runtime_error("triangleIndices.size() is not a multiple of 3");
        }
        if (std::any_of(vertices.begin(), vertices.end(),
                        [](const VertexData &v)
                        { return v.texCoord.x < 0.0f || v.texCoord.x > 1.0f || v.texCoord.y < 0.0f || v.texCoord.y > 1.0f; }))
        {
            throw std::runtime_error("texCoords contains invalid values");
        }
        numTriangles = triangleIndices.size() / 3;
        vbo.addData(vertices.size() * sizeof(VertexData), vertices.data(), gl::GL_STATIC_DRAW);
        ebo.addData(triangleIndices.size() * sizeof(unsigned int), triangleIndices.data(), gl::GL_STATIC_DRAW);
        vao
            .attachVBO(0, vbo.id, 0, sizeof(VertexData))
            .attachEBO(ebo.id);
    }
    const TriangleMesh &TriangleMesh::setVertPosLocation(unsigned int location) const
    {
        vao.addVertAttr(location, 0, 3, gl::GL_FLOAT, gl::GL_FALSE, offsetof(VertexData, pos));
        return *this;
    }
    const TriangleMesh &TriangleMesh::setTexCoordLocation(unsigned int location) const
    {
        vao.addVertAttr(location, 0, 2, gl::GL_FLOAT, gl::GL_FALSE, offsetof(VertexData, texCoord));
        return *this;
    }
    void TriangleMesh::draw() const
    {
        vao.bind();
        gl::glDrawElements(gl::GL_TRIANGLES, numTriangles * 3, gl::GL_UNSIGNED_INT, 0);
    }

}