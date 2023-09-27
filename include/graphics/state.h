#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include "graphics/helper.h"

namespace Graphics
{
    struct Camera
    {
        glm::vec3 pos, focus, up;
        glm::quat orientation;
        glm::mat4 view;
        Camera();
        Camera(const glm::vec3 &pos, const glm::vec3 &focus, const glm::vec3 &up = Direction::UP);
        void updateViewFromVectors();
        void updateOrientationFromView();
    };

    // Singleton class
    struct State
    {
        Camera camera;
        double time = 0.0f;
        double deltaTime = 0.0f;
        double speedFactor = 1e6f;
        static State &ref();

    private:
        State();
        State(const State &) = delete;
        State &operator=(const State &) = delete;
    };
}