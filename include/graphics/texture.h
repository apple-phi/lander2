#include <glbinding/gl/gl.h>
namespace Graphics
{
    struct Tex2D
    {
        gl::GLuint id;

        explicit Tex2D(const std::string &filepath, gl::GLuint nLevels);
        ~Tex2D();
        // https://computergraphics.stackexchange.com/q/10066
        void bind(gl::GLuint texUnitIndex) const;
    };
}