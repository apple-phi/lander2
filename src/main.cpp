#include <iostream>
#include <glBinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glbinding-aux/debug.h>
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

using namespace gl;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (int)GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LanderApp", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Graphics::Callback::framebufferSize);
    glfwSetKeyCallback(window, Graphics::Callback::key);
    glfwSetMouseButtonCallback(window, Graphics::Callback::mouseButton);
    glfwSetCursorPosCallback(window, Graphics::Callback::cursorPos);
    glbinding::initialize(glfwGetProcAddress);
    glbinding::aux::enableGetErrorCallback();
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    auto &state = Graphics::State::ref();
    state.camera = Graphics::Camera(glm::vec3(8e3), glm::vec3(0, 0, 0));

    Graphics::Shader vertexShader2("C:\\Users\\lucas\\OneDrive\\Desktop\\lander2\\src\\graphics\\shaders\\planet.vert", GL_VERTEX_SHADER);
    Graphics::Shader fragmentShader2("C:\\Users\\lucas\\OneDrive\\Desktop\\lander2\\src\\graphics\\shaders\\planet.frag", GL_FRAGMENT_SHADER);
    Graphics::ShaderProgram prog2({vertexShader2, fragmentShader2});

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 2e4f);
    glm::mat4 Model = glm::mat4(1.0);

    const auto &marsMesh = Graphics::Meshes::CubeSphere(8, 3e3)
                               .setVertPosLocation(0)
                               .setTexCoordLocation(1);

    auto colorMap = Graphics::Tex2D("C:/Users/lucas/OneDrive/Desktop/lander2/src/graphics/assets/mars_1k_color.jpg", 10);
    auto normalMap = Graphics::Tex2D("C:/Users/lucas/OneDrive/Desktop/lander2/src/graphics/assets/mars_1k_normal.jpg", 10);
    prog2.use();
    colorMap.bind(0);
    normalMap.bind(1);
    prog2.setUniformTex("colorMap", 0);
    prog2.setUniformTex("normalMap", 1);

    // Graphics::Helper::useWireframe();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.06f, 0.01f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 MVP = Projection * state.camera.view * Model; // Remember, matrix multiplication is the other way around
        prog2.setUniformMat4("MVP", &MVP[0][0]);

        marsMesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
