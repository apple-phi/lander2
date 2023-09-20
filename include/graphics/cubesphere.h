#include <vector>
#include <array>
#include <glbinding/gl/gl.h>
#include <glm/vec3.hpp>
#include "graphics/mesh.h"

namespace Graphics::Meshes
{
    struct CubeSphereFace : public TriangleMesh
    {
        glm::vec3 normal;
        CubeSphereFace();
        CubeSphereFace(const std::vector<VertexData> &vertices, const std::vector<unsigned int> &trianglesIndices, glm::vec3 normal);
        CubeSphereFace(gl::GLuint vbo, gl::GLuint ebo, glm::vec3 normal);
    };
    struct CubeSphere
    {
        std::array<CubeSphereFace, 6> faces;
        explicit CubeSphere(const std::array<CubeSphereFace, 6> &faces);
        explicit CubeSphere(unsigned int resolution);
        void draw() const;
        CubeSphere setVertPosLocation(unsigned int location) const;
        CubeSphere setTexCoordLocation(unsigned int location) const;
    };
}