#pragma once
#include <iostream>
namespace Graphics::Helper
{
    void useWireframe(bool use = true);
    // void print(const glm::vec3 &v);
    // void print(const auto &s);
    void print(const auto &s)
    {
        std::cout << s << std::endl;
    }
    void vecPrint(const glm::vec3 &v);
}

namespace Direction
{
    extern const glm::vec3 UP;
    extern const glm::vec3 DOWN;
    extern const glm::vec3 LEFT;
    extern const glm::vec3 RIGHT;
    extern const glm::vec3 FRONT;
    extern const glm::vec3 BACK;
}
