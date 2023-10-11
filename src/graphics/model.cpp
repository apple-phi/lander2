#include <array>
#include <vector>
#include "graphics/model.h"
#include "graphics/cubesphere.h"
#include "graphics/state.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/buffer.h"
#include "atmosphere/model.h"

// Values from "Reference Solar Spectral Irradiance: ASTM G-173", ETR column
// (see http://rredc.nrel.gov/solar/spectra/am1.5/ASTMG173/ASTMG173.html),
// summed and averaged in each bin (e.g. the value for 360nm is the average
// of the ASTM G-173 values for all wavelengths between 360 and 370nm).
// Values in W.m^-2.
constexpr int kLambdaMin = 360;
constexpr int kLambdaMax = 830;
constexpr double kSolarIrradiance[48] = {
    1.11776, 1.14259, 1.01249, 1.14716, 1.72765, 1.73054, 1.6887, 1.61253,
    1.91198, 2.03474, 2.02042, 2.02212, 1.93377, 1.95809, 1.91686, 1.8298,
    1.8685, 1.8931, 1.85149, 1.8504, 1.8341, 1.8345, 1.8147, 1.78158, 1.7533,
    1.6965, 1.68194, 1.64654, 1.6048, 1.52143, 1.55622, 1.5113, 1.474, 1.4482,
    1.41018, 1.36775, 1.34188, 1.31429, 1.28303, 1.26758, 1.2367, 1.2082,
    1.18737, 1.14683, 1.12362, 1.1058, 1.07124, 1.04992};
// Values from http://www.iup.uni-bremen.de/gruppen/molspec/databases/
// referencespectra/o3spectra2011/index.html for 233K, summed and averaged in
// each bin (e.g. the value for 360nm is the average of the original values
// for all wavelengths between 360 and 370nm). Values in m^2.
constexpr double kOzoneCrossSection[48] = {
    1.18e-27, 2.182e-28, 2.818e-28, 6.636e-28, 1.527e-27, 2.763e-27, 5.52e-27,
    8.451e-27, 1.582e-26, 2.316e-26, 3.669e-26, 4.924e-26, 7.752e-26, 9.016e-26,
    1.48e-25, 1.602e-25, 2.139e-25, 2.755e-25, 3.091e-25, 3.5e-25, 4.266e-25,
    4.672e-25, 4.398e-25, 4.701e-25, 5.019e-25, 4.305e-25, 3.74e-25, 3.215e-25,
    2.662e-25, 2.238e-25, 1.852e-25, 1.473e-25, 1.209e-25, 9.423e-26, 7.455e-26,
    6.566e-26, 5.105e-26, 4.15e-26, 4.228e-26, 3.237e-26, 2.451e-26, 2.801e-26,
    2.534e-26, 1.624e-26, 1.465e-26, 2.078e-26, 1.383e-26, 7.105e-27};
// From https://en.wikipedia.org/wiki/Dobson_unit, in molecules.m^-2.
constexpr double kDobsonUnit = 2.687e20;
// Maximum number density of ozone molecules, in m^-3 (computed so at to get
// 300 Dobson units of ozone - for this we divide 300 DU by the integral of
// the ozone density profile defined below, which is equal to 15km).
constexpr double kMaxOzoneNumberDensity = 300.0 * kDobsonUnit / 15000.0;
// Wavelength independent solar irradiance "spectrum" (not physically
// realistic, but was used in the original implementation).
constexpr double kConstantSolarIrradiance = 1.5;
constexpr double kBottomRadius = 6360000.0;
constexpr double kTopRadius = 6420000.0;
constexpr double kRayleigh = 1.24062e-6;
constexpr double kRayleighScaleHeight = 8000.0;
constexpr double kMieScaleHeight = 1200.0;
constexpr double kMieAngstromAlpha = 0.0;
constexpr double kMieAngstromBeta = 5.328e-3;
constexpr double kMieSingleScatteringAlbedo = 0.9;
constexpr double kMiePhaseFunctionG = 0.8;
constexpr double kGroundAlbedo = 0.1;
const double max_sun_zenith_angle = 102.0 / 180.0 * 3.14159265358979;

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
    void Model::getPos(glm::vec3 &pos) const
    {
        pos = glm::vec3(translationMatrix[3]);
    }
    void Model::setPos(const glm::vec3 &pos)
    {
        translationMatrix[3] = glm::vec4(pos, 1.0f);
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
#include "./shaders/shaders.inc"
        const Graphics::Shader vert(mars_vert, gl::GL_VERTEX_SHADER);
        const Graphics::Shader frag(mars_frag, gl::GL_FRAGMENT_SHADER);
        shaderProgram = Graphics::ShaderProgram({vert, frag});
        colorMap = Graphics::Tex2D("./assets/mars_4k_color.jpg", 10);
        normalMap = Graphics::Tex2D("./assets/mars_4k_normal.jpg", 10);
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
        const auto M = getModelMatrix();
        const glm::mat4 MVP = ViewProj * M;
        shaderProgram.use();
        shaderProgram.setUniformMat4("MVP", &MVP[0][0]);
        shaderProgram.setUniformMat4("M", &M[0][0]);
        gl::glEnable(gl::GL_CULL_FACE);
        for (const auto &mesh : meshes)
        {
            // TODO: only draw if in view frustum
            mesh.draw();
        }
    }
    void Mars::rotateDuringTimeStep(double dt)
    {
        rotate(glm::angleAxis((float)(dt * angularVel), glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    // --------------------------------------------------------------------------------------

    Atmosphere::Atmosphere()
    {
#include "./shaders/shaders.inc"
        const Graphics::Shader vert(atmosphere_vert, gl::GL_VERTEX_SHADER);
        const Graphics::Shader frag(atmosphere_frag, gl::GL_FRAGMENT_SHADER);
        shaderProgram = Graphics::ShaderProgram({vert, frag});
        const auto faces = Graphics::Meshes::CubeSphere(16, outerRadius).faces;
        meshes = std::vector<Meshes::TriangleMesh>(faces.begin(), faces.end());
        this->setVertPosLocation(0);

        // atmosphere::DensityProfileLayer mieLayer;
    }
    void Atmosphere::draw(const glm::mat4 &ViewProj) const
    {
        static const auto &camera = State::ref().camera;
        const glm::mat4 MVP = ViewProj * getModelMatrix();
        shaderProgram.use();
        shaderProgram.setUniformMat4("MVP", &MVP[0][0]);
        shaderProgram.setUniformFloat("innerRadius", innerRadius);
        shaderProgram.setUniformFloat("outerRadius", outerRadius);
        shaderProgram.setUniformVec3("cameraPos", camera.pos);
        // gl::glDisable(gl::GL_CULL_FACE);
        for (const auto &mesh : meshes)
        {
            // TODO: frustum culling
            mesh.draw();
        }
    }

    // --------------------------------------------------------------------------------------

    Lander::Lander()
    {
        vertices = {glm::vec3(0)};
        triangleIndices = {0, 1, 2,
                           0, 2, 3,
                           0, 3, 4,
                           0, 4, 1,
                           1, 3, 2,
                           1, 4, 3};
        auto vbo = Graphics::Buffer();
        // auto ebo = Graphics::Buffer();
        vbo.addData(vertices.size() * sizeof(glm::vec3), vertices.data(), gl::GL_STATIC_DRAW);
        // ebo.addData(triangleIndices.size() * sizeof(unsigned int), triangleIndices.data(), gl::GL_STATIC_DRAW);
        vao = Graphics::VAO()
                  .attachVBO(0, vbo.id, 0, sizeof(glm::vec3));
        //   .attachEBO(ebo.id);
#include "./shaders/shaders.inc"
        const Graphics::Shader vert(lander_vert, gl::GL_VERTEX_SHADER);
        const Graphics::Shader frag(lander_frag, gl::GL_FRAGMENT_SHADER);
        shaderProgram = Graphics::ShaderProgram({vert, frag});
    }
    void Lander::draw(const glm::mat4 &ViewProj) const
    {
        static const auto &camera = State::ref().camera;
        const glm::mat4 MVP = ViewProj * getModelMatrix();
        shaderProgram.use();
        shaderProgram.setUniformMat4("MVP", &MVP[0][0]);
        vao.bind();
        gl::glDrawArrays(gl::GL_POINTS, 0, 1);
    }
}