#include "graphics/model.h"
#include "graphics/cubesphere.h"
#include "graphics/state.h"

namespace Graphics
{
    Model::Model(const std::vector<Meshes::TriangleMesh> &meshes) : meshes(meshes) {}
    void Model::draw() const
    {
        for (const auto &mesh : meshes)
        {
            mesh.draw();
        }
    }
    const Model &Model::setVertPosLocation(unsigned int location) const
    {
        for (const auto &mesh : meshes)
        {
            mesh.setVertPosLocation(location);
        }
        return *this;
    }
    const Model &Model::setTexCoordLocation(unsigned int location) const
    {
        for (const auto &mesh : meshes)
        {
            mesh.setTexCoordLocation(location);
        }
        return *this;
    }
    void Model::translate(const glm::vec3 &translation)
    {
        translationMatrix[3] += glm::vec4(translation, 0.0f);
    }
    void Model::rotate(const glm::quat &rotation)
    {
        orientation = rotation * orientation;
    }
    glm::mat4 Model::getModelMatrix() const
    {
        // TODO: implement rotation matrix
        // NOTE: maybe implement caching
        return translationMatrix * glm::mat4_cast(orientation);
    }

    // --------------------------------------------------------------------------------------

    Mars::Mars()
    {
        const Graphics::Shader vert("C:\\Users\\lucas\\OneDrive\\Desktop\\lander2\\src\\graphics\\shaders\\mars.vert", gl::GL_VERTEX_SHADER);
        const Graphics::Shader frag("C:\\Users\\lucas\\OneDrive\\Desktop\\lander2\\src\\graphics\\shaders\\mars.frag", gl::GL_FRAGMENT_SHADER);
        shaderProgram = Graphics::ShaderProgram({vert, frag});
        colorMap = Graphics::Tex2D("C:/Users/lucas/OneDrive/Desktop/lander2/src/graphics/assets/mars_1k_color.jpg", 10);
        normalMap = Graphics::Tex2D("C:/Users/lucas/OneDrive/Desktop/lander2/src/graphics/assets/mars_1k_normal.jpg", 10);
        shaderProgram.use();
        colorMap.bind(0);
        normalMap.bind(1);
        shaderProgram.setUniformTex("colorMap", 0);
        shaderProgram.setUniformTex("normalMap", 1);
        const auto faces = Graphics::Meshes::CubeSphere(16, radius).faces;
        meshes = std::vector<Meshes::TriangleMesh>(faces.begin(), faces.end());
        this->setVertPosLocation(0);
        this->setTexCoordLocation(1);
    }
    void Mars::draw(const glm::mat4 &ViewProj) const
    {
        static const auto &camera = State::ref().camera;
        const glm::mat4 MVP = ViewProj * getModelMatrix();
        shaderProgram.use();
        shaderProgram.setUniformMat4("MVP", &MVP[0][0]);
        for (const auto &mesh : meshes)
        {
            // TODO: only draw if in view frustum
            mesh.draw();
        }
    }
}