#include <iostream>
#include <glm/vec3.hpp>
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

static_assert(sizeof(glm::vec3) == sizeof(float) * 3, "glm::vec3 is not the same size as float[3]");
namespace Graphics
{
    TriangleMesh::TriangleMesh(const std::vector<glm::vec3> &vertices, const std::vector<unsigned int> &triangleIndices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texCoords)
        : vertices{std::move(vertices)}, triangleIndices{std::move(triangleIndices)}, normals{std::move(normals)}, texCoords{std::move(texCoords)}
    {
        if (triangleIndices.size() % 3 != 0)
        {
            throw std::runtime_error("triangleIndices.size() is not a multiple of 3");
        }
        vbo.addData(vertices.size() * sizeof(glm::vec3), vertices.data(), gl::GL_STATIC_DRAW);
        ebo.addData(triangleIndices.size() * sizeof(unsigned int), triangleIndices.data(), gl::GL_STATIC_DRAW);
        vao
            .attachVBO(0, vbo.id, 0, sizeof(glm::vec3))
            .attachEBO(ebo.id);
    }
    void TriangleMesh::setVertPosLocation(unsigned int location) const
    {
        vao.addVertAttr(location, 0, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);
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
        std::vector<glm::vec3> vertices(resolution * resolution);
        std::vector<unsigned int> triangles((resolution - 1) * (resolution - 1) * 6);
        std::vector<glm::vec3> normals(resolution * resolution);
        std::vector<glm::vec2> texCoords(resolution * resolution);
        size_t triIndex = 0;
        for (unsigned int i = 0; i < resolution; i++)
        {
            for (unsigned int j = 0; j < resolution; j++)
            {
                const size_t vertIndex = i + j * resolution;
                const glm::vec2 t = glm::vec2(i, j) / (resolution - 1.0f);
                const glm::vec3 p = normal + tangent * 2.0f * (t.x - 0.5f) + bitangent * 2.0f * (t.y - 0.5f);
                vertices[vertIndex] = mapCubePointToSphere(p);
                normals[vertIndex] = glm::normalize(p);
                texCoords[vertIndex] = {i / ((double)resolution - 1), j / ((double)resolution - 1)};
                if (i < resolution - 1 && j < resolution - 1)
                {
                    triangles[triIndex++] = vertIndex;
                    triangles[triIndex++] = vertIndex + resolution + 1;
                    triangles[triIndex++] = vertIndex + resolution;
                    triangles[triIndex++] = vertIndex;
                    triangles[triIndex++] = vertIndex + 1;
                    triangles[triIndex++] = vertIndex + resolution + 1;
                }
            }
        }
        return {vertices, triangles, normals, texCoords};
    }
}