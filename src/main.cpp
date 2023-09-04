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

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    Graphics::Shader vertexShader("C:\\Users\\lucas\\OneDrive\\Desktop\\lander2\\src\\graphics\\shaders\\test.vert", GL_VERTEX_SHADER);
    Graphics::Shader fragmentShader("C:\\Users\\lucas\\OneDrive\\Desktop\\lander2\\src\\graphics\\shaders\\test.frag", GL_FRAGMENT_SHADER);
    Graphics::ShaderProgram prog({vertexShader, fragmentShader});

    const std::vector<glm::vec3> vertices = {
        {0.5f, 0.5f, 0.0f},   // top right
        {0.5f, -0.5f, 0.0f},  // bottom right
        {-0.5f, -0.5f, 0.0f}, // bottom left
        {-0.5f, 0.5f, 0.0f},  // top left
    };
    const std::vector<unsigned int> indices = {
        0,
        1,
        3, // 1st triangle
        1,
        2,
        3, // 2nd triangle
    };
    Graphics::TriangleMesh mesh(vertices, indices);
    mesh.setVertPosLocation(0);

    // uncomment this call to draw in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prog.use();
        mesh.draw();

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
