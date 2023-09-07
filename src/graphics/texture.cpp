#include <iostream>
#include <fstream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "graphics/texture.h"

namespace Graphics
{
    Tex2D::Tex2D(const std::string &filepath, gl::GLuint nLevels)
    {
        int width, height, nChannels;
        // stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nChannels, 0);
        if (data == NULL)
        {
            std::cerr << "Failed to load image: " << filepath << std::endl;
            throw std::runtime_error(stbi_failure_reason());
        }
        std::cout << "Loaded image: " << filepath << std::endl;
        std::cout << width << " " << height << " " << nChannels << std::endl;
        gl::GLenum format;
        gl::GLenum internalFormat;
        switch (nChannels)
        {
        case 1:
            format = gl::GL_RED;
            internalFormat = gl::GL_R8;
            break;
        case 2:
            format = gl::GL_RG;
            internalFormat = gl::GL_RG8;
            break;
        case 3:
            format = gl::GL_RGB;
            internalFormat = gl::GL_RGB8;
            break;
        case 4:
            format = gl::GL_RGBA;
            internalFormat = gl::GL_RGBA8;
            break;
        default:
            std::cerr << "Invalid number of channels: " << nChannels << std::endl;
            throw std::runtime_error("Invalid number of channels");
        }
        // https://stackoverflow.com/q/66574354
        gl::glCreateTextures(gl::GL_TEXTURE_2D, 1, &id);
        gl::glTextureStorage2D(id, nLevels, internalFormat, width, height);
        gl::glTextureSubImage2D(id, 0, 0, 0, width, height, format, gl::GL_UNSIGNED_BYTE, data);
        gl::glTextureParameteri(id, gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
        gl::glTextureParameteri(id, gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
        gl::glTextureParameteri(id, gl::GL_TEXTURE_WRAP_R, gl::GL_CLAMP_TO_EDGE);
        gl::glTextureParameteri(id, gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
        gl::glTextureParameteri(id, gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
        gl::glGenerateTextureMipmap(id);
        stbi_image_free(data);
    }
    Tex2D::~Tex2D()
    {
        // gl::glDeleteTextures(1, &id);
    }
    void Tex2D::bind(gl::GLuint texUnitIndex) const
    {
        gl::glBindTextureUnit(texUnitIndex, id);
    }
}