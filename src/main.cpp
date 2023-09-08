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

using namespace gl;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glbinding::initialize(glfwGetProcAddress);
    glbinding::aux::enableGetErrorCallback();
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    Graphics::Shader vertexShader2("C:\\Users\\lucas\\OneDrive\\Desktop\\lander2\\src\\graphics\\shaders\\planet.vert", GL_VERTEX_SHADER);
    Graphics::Shader fragmentShader2("C:\\Users\\lucas\\OneDrive\\Desktop\\lander2\\src\\graphics\\shaders\\planet.frag", GL_FRAGMENT_SHADER);
    Graphics::ShaderProgram prog2({vertexShader2, fragmentShader2});

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
        glm::vec3(3, 3, -3), // Camera is at (4,3,-3), in World Space
        glm::vec3(0, 0, 0),  // and looks at the origin
        glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 Model = glm::mat4(1.0);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

    const auto &shaderMeshes = Graphics::shaderGenerateFaces(200);
    for (const auto &m : shaderMeshes)
    {
        m.setVertPosLocation(0);
        m.setTexCoordLocation(1);
    }

    auto colorMap = Graphics::Tex2D("C:/Users/lucas/OneDrive/Desktop/lander2/src/graphics/assets/mars_1k_color.jpg", 10);

    // Graphics::Helper::useWireframe();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.06f, 0.01f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prog2.use();
        prog2.setUniformMat4("MVP", &MVP[0][0]);
        colorMap.bind(0);
        for (const auto &m : shaderMeshes)
        {
            m.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
