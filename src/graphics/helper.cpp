#include <glbinding/gl/gl.h>
namespace Graphics::Helper
{
    void useWireframe(bool use)
    {
        use ? gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_LINE)
            : gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_FILL);
    };
}