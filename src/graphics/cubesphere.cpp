#include <stdexcept>
#include "graphics/cubesphere.h"
#include "graphics/shader.h"
#include "graphics/helper.h"

namespace Graphics::Meshes
{
    CubeSphereFace::CubeSphereFace(const std::vector<VertexData> &vertices, const std::vector<unsigned int> &trianglesIndices, glm::vec3 normal)
        : TriangleMesh(vertices, trianglesIndices), normal(normal) {}
    CubeSphereFace::CubeSphereFace(gl::GLuint vbo, gl::GLuint ebo, glm::vec3 normal)
        : TriangleMesh(vbo, ebo), normal(normal) {}

    CubeSphere::CubeSphere(const std::array<CubeSphereFace, 6> &faces)
        : faces(faces) {}
    CubeSphere::CubeSphere(unsigned int resolution)
    {
        if (resolution % 8 != 0)
        {
            throw std::runtime_error("resolution is not a multiple of 8");
        }
        const Shader computeShader("C:/Users/lucas/OneDrive/Desktop/lander2/src/graphics/shaders/cubesphere.comp", gl::GL_COMPUTE_SHADER);
        const ShaderProgram prog({computeShader});
        prog.use();
        prog.setUniformUnsignedInt("resolution", resolution);
        const std::vector<glm::vec3> directions = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT, Direction::FRONT, Direction::BACK};
        std::transform(directions.begin(), directions.end(), faces.begin(), [&](const glm::vec3 &cubeFaceNormal)
                       {prog.setUniformVec3("normal", cubeFaceNormal);
                        Buffer vertices, triangleIndices;
                        // https://stackoverflow.com/a/56470902
                        vertices.addData(resolution * resolution * sizeof(VertexData), nullptr, gl::GL_DYNAMIC_DRAW);
                        triangleIndices.addData((resolution - 1) * (resolution - 1) * 6 * sizeof(unsigned int), nullptr, gl::GL_DYNAMIC_DRAW);
                        gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, vertices.id);
                        gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, gl::glGetProgramResourceIndex(prog.id, gl::GL_SHADER_STORAGE_BLOCK, "vertexBlock"), vertices.id);
                        gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, triangleIndices.id);
                        gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, gl::glGetProgramResourceIndex(prog.id, gl::GL_SHADER_STORAGE_BLOCK, "triangleIndexBlock"), triangleIndices.id);

                        // https://forum.unity.com/threads/compute-shader-thread-dispatching.953000/#post-6211889
                        // https://stackoverflow.com/a/62601514
                        gl::glDispatchCompute(resolution/8, resolution/8, 1);
                        // gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);
                        // std::cout << vertices.getSubData<VertexData>(0).texCoord.x << std::endl;
                        return std::move(CubeSphereFace(vertices.id, triangleIndices.id, cubeFaceNormal)); });
        gl::glMemoryBarrier(gl::GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT |
                            gl::GL_ELEMENT_ARRAY_BARRIER_BIT |
                            gl::GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                            gl::GL_TEXTURE_FETCH_BARRIER_BIT);
    }
    void CubeSphere::draw() const
    {
        for (const auto &face : faces)
        {
            face.draw();
        }
    }
    const CubeSphere &CubeSphere::setVertPosLocation(unsigned int location) const
    {
        for (auto &face : faces)
        {
            face.setVertPosLocation(location);
        }
        return *this;
    }
    const CubeSphere &CubeSphere::setTexCoordLocation(unsigned int location) const
    {
        for (auto &face : faces)
        {
            face.setTexCoordLocation(location);
        }
        return *this;
    }
}