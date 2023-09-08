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
    TriangleMesh::TriangleMesh() {}
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
        gl::glDrawElements(gl::GL_TRIANGLES, numTriangles * 3, gl::GL_UNSIGNED_INT, 0);
    }

    //------------------------------------------------------------------------------------

    std::array<TriangleMesh, 6> shaderGenerateFaces(unsigned int resolution)
    {
        if (resolution % 8 != 0)
        {
            throw std::runtime_error("resolution is not a multiple of 8");
        }
        std::array<TriangleMesh, 6> meshes;
        const std::vector<glm::vec3> directions = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT, Direction::FRONT, Direction::BACK};
        Shader computeShader("C:/Users/lucas/OneDrive/Desktop/lander2/src/graphics/shaders/sphereMeshFace.comp", gl::GL_COMPUTE_SHADER);
        ShaderProgram prog({computeShader});
        prog.setUniformUnsignedInt("resolution", resolution);
        std::transform(directions.begin(), directions.end(), meshes.begin(), [&](const glm::vec3 &cubeFaceNormal)
                       {prog.setUniformVec3("normal", cubeFaceNormal);
                        Buffer vertices, triangleIndices;
                        // https://stackoverflow.com/a/56470902
                        vertices.addData(resolution * resolution * sizeof(VertexData), nullptr, gl::GL_DYNAMIC_DRAW);
                        triangleIndices.addData((resolution - 1) * (resolution - 1) * 6 * sizeof(unsigned int), nullptr, gl::GL_DYNAMIC_DRAW);
                        gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, vertices.id);
                        gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, gl::glGetProgramResourceIndex(prog.id, gl::GL_SHADER_STORAGE_BLOCK, "vertexBlock"), vertices.id);
                        gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, triangleIndices.id);
                        gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, gl::glGetProgramResourceIndex(prog.id, gl::GL_SHADER_STORAGE_BLOCK, "triangleIndexBlock"), triangleIndices.id);
                        prog.use();
                        // https://forum.unity.com/threads/compute-shader-thread-dispatching.953000/#post-6211889
                        // https://stackoverflow.com/a/62601514
                        gl::glDispatchCompute(resolution/8, resolution/8, 1);
                        return std::move(TriangleMesh{vertices.id, triangleIndices.id}); });
        gl::glMemoryBarrier(gl::GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | gl::GL_ELEMENT_ARRAY_BARRIER_BIT);
        return meshes;
    }
    TriangleMesh shaderMakeSpherifiedCubeFace(const glm::vec3 &normal, unsigned int resolution)
    {
        if (resolution % 8 != 0)
        {
            throw std::runtime_error("resolution is not a multiple of 8");
        }
        Shader computeShader("C:/Users/lucas/OneDrive/Desktop/lander2/src/graphics/shaders/sphereMeshFace.comp", gl::GL_COMPUTE_SHADER);
        ShaderProgram prog({computeShader});
        Buffer vertices, triangleIndices;
        /// https://stackoverflow.com/a/56470902
        vertices.addData(resolution * resolution * sizeof(VertexData), nullptr, gl::GL_DYNAMIC_DRAW);
        triangleIndices.addData((resolution - 1) * (resolution - 1) * 6 * sizeof(unsigned int), nullptr, gl::GL_DYNAMIC_DRAW);
        gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, vertices.id);
        gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, gl::glGetProgramResourceIndex(prog.id, gl::GL_SHADER_STORAGE_BLOCK, "vertexBlock"), vertices.id);
        gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, triangleIndices.id);
        gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, gl::glGetProgramResourceIndex(prog.id, gl::GL_SHADER_STORAGE_BLOCK, "triangleIndexBlock"), triangleIndices.id);
        prog.setUniformUnsignedInt("resolution", resolution);
        prog.setUniformVec3("normal", normal);
        prog.use();
        gl::glDispatchCompute(resolution / 8, resolution / 8, 1);
        gl::glMemoryBarrier(gl::GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
        gl::glMemoryBarrier(gl::GL_ELEMENT_ARRAY_BARRIER_BIT);
        return std::move(TriangleMesh{vertices.id, triangleIndices.id});
    }

    // -----------------------------------------------------------------------------------

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