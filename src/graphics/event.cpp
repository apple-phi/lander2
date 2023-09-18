#include <glbinding/gl/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <cmath>
#include <numbers>

#include "graphics/event.h"
#include "graphics/helper.h"
#include "graphics/state.h"

// Project an x,y pair onto a sphere of radius r or a hyperbolic sheet if
// we are away from the centre of the sphere
// Use this to avoid gimbal lock in quaternion camera orientation
inline glm::vec3 projectOnTrackball(float r, glm::vec2 screenPos)
{
    const float d = glm::length(screenPos);
    const float t = r / std::numbers::sqrt2;
    if (d < t)
    { // inside sphere
        return glm::vec3(screenPos, std::sqrt(r * r - d * d));
    }
    // on hyperbola
    return glm::vec3(screenPos, t * t / d);
}

namespace Graphics::Callback
{
    bool _mouseDragging = false;
    double _dragSensitivity = 0.01,
           trackballRadius = 0.8;
    glm::vec3 lastTrackballProjection;

    void mouseButton(GLFWwindow *window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                _mouseDragging = true;
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                lastTrackballProjection = projectOnTrackball(trackballRadius, {2.0 * x / 800.0 - 1.0, 1.0 - 2.0 * y / 600.0});
            }
            else if (action == GLFW_RELEASE)
            {
                _mouseDragging = false;
            }
        }
    }

    void cursorPos(GLFWwindow *window, double xPos, double yPos)
    {
        if (_mouseDragging)
        {
            auto &state = State::ref();
            glm::vec3 newProjection(projectOnTrackball(trackballRadius, {2.0 * xPos / 800.0 - 1.0, 1.0 - 2.0 * yPos / 600.0}));
            float angle = glm::acos(glm::clamp(glm::dot(glm::normalize(newProjection), glm::normalize(lastTrackballProjection)), 0.0f, 1.0f));
            const glm::mat4 invView = glm::inverse(state.camera.view);
            glm::vec3 oldProjectionCameraSpace = invView * glm::vec4(lastTrackballProjection, 1.0);
            glm::vec3 newProjectionCameraSpace = invView * glm::vec4(newProjection, 1.0);
            const glm::vec3 axis = glm::normalize(glm::cross(newProjectionCameraSpace, oldProjectionCameraSpace));
            const glm::quat deltaOrientation = glm::angleAxis(angle, axis);
            state.camera.pos = glm::rotate(deltaOrientation, state.camera.pos - state.camera.focus) + state.camera.focus;
            state.camera.up = glm::rotate(deltaOrientation, state.camera.up);
            state.camera.updateViewFromVectors();
            lastTrackballProjection = newProjection;
        }
    }

    void key(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    void framebufferSize(GLFWwindow *window, int width, int height)
    {
        gl::glViewport(0, 0, width, height);
    }
}