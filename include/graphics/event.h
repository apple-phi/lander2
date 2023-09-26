#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Graphics::Callback
{
    void mouseButton(GLFWwindow *window, int button, int action, int mods);
    void scroll(GLFWwindow *window, double xOffset, double yOffset);
    void cursorPos(GLFWwindow *window, double xPos, double yPos);
    void key(GLFWwindow *window, int key, int scancode, int action, int mods);
    void framebufferSize(GLFWwindow *window, int width, int height);
}