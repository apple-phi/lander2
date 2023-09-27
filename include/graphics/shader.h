#pragma once
#include <string>
#include <initializer_list>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

namespace Graphics
{
    struct Shader
    {
        gl::GLuint id;
        gl::GLenum type;
        Shader(const char *shaderSource, gl::GLenum type);
        // Shader(const std::string &filepath, gl::GLenum type);
        ~Shader();
        gl::GLint getParameter(gl::GLenum pname) const;
    };

    struct ShaderProgram
    {
        gl::GLuint id;
        ShaderProgram() = default;
        explicit ShaderProgram(std::initializer_list<Shader> shaders);
        ~ShaderProgram();
        void use() const;
        gl::GLint getParameter(gl::GLenum pname) const;
        void setUniformBool(const std::string &name, bool value) const;
        void setUniformUnsignedInt(const std::string &name, unsigned int value) const;
        void setUniformInt(const std::string &name, int value) const;
        void setUniformFloat(const std::string &name, float value) const;
        void setUniformMat3(const std::string &name, const gl::GLfloat *mat) const;
        void setUniformMat4(const std::string &name, const gl::GLfloat *mat) const;
        void setUniformVec2(const std::string &name, glm::vec2 vec) const;
        void setUniformVec3(const std::string &name, glm::vec3 vec) const;
        void setUniformVec4(const std::string &name, glm::vec4 vec) const;
        void setUniformTex(const std::string &name, gl::GLuint texUnitIndex) const;
    };

}