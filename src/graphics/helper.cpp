#include <glbinding/gl/gl.h>
#include <glm/vec3.hpp>
#include "graphics/helper.h"
namespace Graphics::Helper
{
    void useWireframe(bool use)
    {
        use ? gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_LINE)
            : gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_FILL);
    };
}

namespace Direction
{
    const glm::vec3 UP{0, 1, 0};
    const glm::vec3 DOWN{0, -1, 0};
    const glm::vec3 LEFT{-1, 0, 0};
    const glm::vec3 RIGHT{1, 0, 0};
    const glm::vec3 FRONT{0, 0, 1};
    const glm::vec3 BACK{0, 0, -1};
}