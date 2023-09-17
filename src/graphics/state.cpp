#include <glm/vec3.hpp>
#include "graphics/state.h"
#include "graphics/helper.h"
namespace Graphics
{
    Camera::Camera() : Camera(Direction::BACK, glm::vec3()) {}
    Camera::Camera(const glm::vec3 &pos, const glm::vec3 &focus, const glm::vec3 &up)
        : pos(pos), focus(focus), up(up)
    {
        updateViewFromVectors();
        updateOrientationFromView();
    }

    void Camera::updateViewFromVectors()
    {
        view = glm::lookAt(pos, focus, up);
    }
    void Camera::updateOrientationFromView()
    {
        orientation = glm::conjugate(glm::toQuat(view));
    }

    State::State() : camera() {}
    // Guaranteed to be thread-safe in C++11 and later
    State &State::ref()
    {
        static State instance;
        return instance;
    }
}