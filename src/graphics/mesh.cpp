#include <iostream>
#include <algorithm>
#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include "graphics/mesh.h"

using glm::vec3;

namespace Direction
{
    const vec3 UP{0, 1, 0};
    const vec3 DOWN{0, -1, 0};
    const vec3 LEFT{-1, 0, 0};
    const vec3 RIGHT{1, 0, 0};
    const vec3 FRONT{0, 0, 1};
    const vec3 BACK{0, 0, -1};
}

// http://mathproofs.blogspot.com/2005/07/mapping-cube-to-sphere.html
vec3 mapCubePointToSphere(const vec3 &p)
{
    const vec3 v = p * p * 0.5f;
    return p * glm::sqrt(1.0f + vec3{
                                    -v.y - v.z + p.y * p.y * p.z * p.z / 3.0f,
                                    -v.z - v.x + p.z * p.z * p.x * p.x / 3.0f,
                                    -v.x - v.y + p.x * p.x * p.y * p.y / 3.0f,
                                });
}

// https://gamedev.stackexchange.com/a/114416
glm::vec2 mapUnitSpherePointToUV(const glm::vec3 &p)
{
    // if (std::abs(glm::length(p) - 1.0) >= 1e-6)
    // {
    //     std::cout << "ERROR in mapUnitSpherePointToUV with p = (" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
    //     throw std::runtime_error("p is not a point on the unit sphere.");
    // }
    return glm::vec2{
        glm::atan(-p.x, -p.z) / glm::two_pi<float>() + 0.5f,
        -p.y * 0.5 + 0.5};
}

static_assert(sizeof(glm::vec3) == sizeof(float) * 3, "glm::vec3 is not the same size as float[3]");
static_assert(sizeof(Graphics::VertexData) == sizeof(float) * 5, "VertexData is not the same size as float[5]");
namespace Graphics
{
    TriangleMesh::TriangleMesh(const std::vector<VertexData> &vertices, const std::vector<unsigned int> &triangleIndices)
        : vertices{std::move(vertices)}, triangleIndices{std::move(triangleIndices)}
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
        vbo.addData(vertices.size() * sizeof(VertexData), vertices.data(), gl::GL_STATIC_DRAW);
        ebo.addData(triangleIndices.size() * sizeof(unsigned int), triangleIndices.data(), gl::GL_STATIC_DRAW);
        vao
            .attachVBO(0, vbo.id, 0, sizeof(VertexData))
            .attachEBO(ebo.id);
    }
    void TriangleMesh::setVertPosLocation(unsigned int location) const
    {
        vao.addVertAttr(location, 0, 3, gl::GL_FLOAT, gl::GL_FALSE, offsetof(VertexData, pos));
    }
    void TriangleMesh::setTexCoordLocation(unsigned int location) const
    {
        vao.addVertAttr(location, 0, 2, gl::GL_FLOAT, gl::GL_FALSE, offsetof(VertexData, texCoord));
    }
    void TriangleMesh::draw() const
    {
        vao.bind();
        gl::glDrawElements(gl::GL_TRIANGLES, triangleIndices.size(), gl::GL_UNSIGNED_INT, 0);
    }

    //------------------------------------------------------------------------------------

    std::array<TriangleMesh, 6> generateFaces(unsigned int resolution)
    {
        return {
            makeSpherifiedCubeFace(Direction::UP, resolution),
            makeSpherifiedCubeFace(Direction::DOWN, resolution),
            makeSpherifiedCubeFace(Direction::LEFT, resolution),
            makeSpherifiedCubeFace(Direction::RIGHT, resolution),
            makeSpherifiedCubeFace(Direction::FRONT, resolution),
            makeSpherifiedCubeFace(Direction::BACK, resolution)};
    };
    TriangleMesh makeSpherifiedCubeFace(const glm::vec3 &normal, unsigned int resolution)
    {
        glm::vec3 tangent = {normal.y, normal.z, normal.x};
        glm::vec3 bitangent = glm::cross(normal, tangent);
        std::vector<VertexData> vertices(resolution * resolution);
        std::vector<unsigned int> triangleIndices((resolution - 1) * (resolution - 1) * 6);
        for (unsigned int i = 0; i < resolution; i++)
        {
            for (unsigned int j = 0; j < resolution; j++)
            {
                const size_t vertIndex = i + j * resolution;
                const glm::vec2 t = glm::vec2(i, j) / (resolution - 1.0f);
                const glm::vec3 p = normal + tangent * 2.0f * (t.x - 0.5f) + bitangent * 2.0f * (t.y - 0.5f);
                const glm::vec3 spherePoint = mapCubePointToSphere(p);
                vertices[vertIndex] = {.pos = spherePoint, .texCoord = mapUnitSpherePointToUV(spherePoint)};
                if (i < resolution - 1 && j < resolution - 1)
                {
                    size_t triIndex = 6 * (i * (resolution - 1) + j);
                    triangleIndices[triIndex++] = vertIndex;
                    triangleIndices[triIndex++] = vertIndex + resolution + 1;
                    triangleIndices[triIndex++] = vertIndex + resolution;
                    triangleIndices[triIndex++] = vertIndex;
                    triangleIndices[triIndex++] = vertIndex + 1;
                    triangleIndices[triIndex] = vertIndex + resolution + 1;
                }
            }
        }
        return std::move(TriangleMesh{vertices, triangleIndices});
    }
}