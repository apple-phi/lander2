#pragma once
#include <glbinding/gl/gl.h>
namespace Graphics
{
    struct Tex2D
    {
        gl::GLuint id, nLevels;
        int width = 0,
            height = 0,
            nChannels = 0;
        gl::GLenum format, internalFormat;

        Tex2D() = default;
        explicit Tex2D(const std::string &filepath);
        // If nLevels is 0, then it will be calculated automatically.
        Tex2D(const std::string &filepath, gl::GLuint nLevels);
        ~Tex2D();
        // https://computergraphics.stackexchange.com/q/10066
        void bind(gl::GLuint texUnitIndex) const;

    private:
        void chooseFormats();
    };
}