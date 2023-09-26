#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"

namespace Graphics
{
    // NOTE: doesn't support scaling
    struct Model
    {
        std::vector<Meshes::TriangleMesh> meshes;
        glm::mat4 translationMatrix = glm::mat4(1.0f);
        glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        Model() = default;
        explicit Model(const std::vector<Meshes::TriangleMesh> &meshes);
        void draw() const;
        const Model &setVertPosLocation(unsigned int location) const;
        const Model &setTexCoordLocation(unsigned int location) const;
        void translate(const glm::vec3 &translation);
        void rotate(const glm::quat &rotation);
        glm::mat4 getModelMatrix() const;
    };

    struct Mars : public Model
    {
        ShaderProgram shaderProgram;
        Tex2D heightMap, normalMap, colorMap;
        float radius = 3389.5;               // km from center at datum
        float highestPointAboveDatum = 21.9; // km +ve , Olympus Mons
        float lowestPointBelowDatum = 7.152; // km -ve , Hellas Planitia

        Mars();
        void draw(const glm::mat4 &ViewProj) const;
    };

    struct Atmosphere : public Model
    {
        ShaderProgram shaderProgram;
        float outerRadius = 4000;
        float innerRadius = 3389.5; // Mars datum

        Atmosphere();
        void draw(const glm::mat4 &ViewProj) const;
    };
}
