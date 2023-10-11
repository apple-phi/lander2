#include <iostream>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
// #include <glbinding-aux/debug.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/buffer.h"
#include "graphics/mesh.h"
#include "graphics/helper.h"
#include "graphics/texture.h"
#include "graphics/event.h"
#include "graphics/state.h"
#include "graphics/cubesphere.h"
#include "graphics/model.h"
#include "physics/state.h"

using namespace gl;

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main(int argc, char *argv[])
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        const char *error[512];
        glfwGetError(&error[0]);
        std::cout << error << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (int)GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LanderApp", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        const char *error[512];
        glfwGetError(&error[0]);
        std::cout << error << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Graphics::Callback::framebufferSize);
    glfwSetKeyCallback(window, Graphics::Callback::key);
    glfwSetMouseButtonCallback(window, Graphics::Callback::mouseButton);
    glfwSetCursorPosCallback(window, Graphics::Callback::cursorPos);
    glfwSetScrollCallback(window, Graphics::Callback::scroll);
    glbinding::initialize(glfwGetProcAddress);
    // glbinding::aux::enableGetErrorCallback();
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    auto &gstate = Graphics::State::ref();
    auto &pstate = Physics::State::ref();

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2e4f);
    auto mars = Graphics::Mars();
    gstate.camera = Graphics::Camera(glm::vec3(1.3e4, 0, 0), glm::vec3(0, 0, 0));
    // auto atmosphere = Graphics::Atmosphere();
    auto glander = Graphics::Lander();
    glander.translate(glm::vec3(7e3, 0, 0));

    pstate.lander = Physics::Lander({3399500, 0, 0}, {0, 0, 0});

    // Graphics::Helper::useWireframe();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.00f, 0.00f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 VP = Projection * gstate.camera.view;
        mars.draw(VP);
        glander.draw(VP);
        // atmosphere.draw(VP);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // if (pstate.lander.status == Physics::LanderStatus::LANDED)
        //     continue;
        // pstate.time += 0.1;
        // pstate.lander.updateDynamics(0.1);
        // // Graphics::Helper::vecPrint(pstate.lander.pos);
        // if (std::isnan(glm::length(pstate.lander.pos)))
        // {
        //     std::cout << "NaN" << std::endl;
        //     break;
        // }
        // Graphics::Helper::print(pstate.lander.altitude());
        // Graphics::Helper::print(pstate.time);

        const double t = glfwGetTime();
        gstate.deltaTime = t - gstate.time;
        gstate.time = t;
        mars.rotateDuringTimeStep(gstate.speedFactor * gstate.deltaTime);
    }
    glfwTerminate();
    return 0;
}
