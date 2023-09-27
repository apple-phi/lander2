#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "graphics/shader.h"

namespace Graphics
{
    Shader::Shader(const char *shaderSource, gl::GLenum type)
    {
        this->id = gl::glCreateShader(type);
        gl::glShaderSource(id, 1, &shaderSource, nullptr);
        gl::glCompileShader(id);
        if (!getParameter(gl::GL_COMPILE_STATUS))
        {
            const gl::GLsizei logSize = getParameter(gl::GL_INFO_LOG_LENGTH);
            char infoLog[logSize];
            gl::glGetShaderInfoLog(id, logSize, nullptr, &infoLog[0]);
            std::cout << "Shader compilation failed for "
                      << shaderSource
                      << std::endl;
            throw std::runtime_error(infoLog);
        }
    }
    Shader::Shader(const std::string &filepath, gl::GLenum type) : type(type)
    {
        std::ifstream file(filepath);
        std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        const char *sourcePtr = source.c_str();
        Shader(sourcePtr, type);
    }
    Shader::~Shader()
    {
        // gl::glDeleteShader(id);
    }
    gl::GLint Shader::getParameter(gl::GLenum pname) const
    {
        gl::GLint param;
        gl::glGetShaderiv(id, pname, &param);
        return param;
    }

    // ------------------------------------------------------------------------

    ShaderProgram::ShaderProgram(std::initializer_list<Shader> shaders)
    {
        this->id = gl::glCreateProgram();
        for (const auto shader : shaders)
        {
            gl::glAttachShader(id, shader.id);
        }
        gl::glLinkProgram(id);
        if (!getParameter(gl::GL_LINK_STATUS))
        {
            const size_t logSize = getParameter(gl::GL_INFO_LOG_LENGTH);
            char infoLog[logSize];
            gl::glGetProgramInfoLog(id, logSize, nullptr, infoLog);
            std::cout << "Shader program linking failed:"
                      << std::endl;
            throw std::runtime_error(infoLog);
        }
        for (const auto shader : shaders)
        {
            gl::glDetachShader(id, shader.id);
        }
    }
    ShaderProgram::~ShaderProgram()
    {
        // gl::glDeleteProgram(id);
    }
    void ShaderProgram::use() const
    {
        gl::glUseProgram(id);
    }
    gl::GLint ShaderProgram::getParameter(gl::GLenum pname) const
    {
        gl::GLint param;
        gl::glGetProgramiv(id, pname, &param);
        return param;
    }
    void ShaderProgram::setUniformBool(const std::string &name, bool value) const
    {
        gl::glProgramUniform1i(id, gl::glGetUniformLocation(id, name.c_str()), (int)value);
    }
    void ShaderProgram::setUniformUnsignedInt(const std::string &name, unsigned int value) const
    {
        gl::glProgramUniform1ui(id, gl::glGetUniformLocation(id, name.c_str()), value);
    }
    void ShaderProgram::setUniformInt(const std::string &name, int value) const
    {
        gl::glProgramUniform1i(id, gl::glGetUniformLocation(id, name.c_str()), value);
    }
    void ShaderProgram::setUniformFloat(const std::string &name, float value) const
    {
        gl::glProgramUniform1f(id, gl::glGetUniformLocation(id, name.c_str()), value);
    }
    void ShaderProgram::setUniformMat3(const std::string &name, const gl::GLfloat *mat) const
    {
        gl::glProgramUniformMatrix3fv(id, gl::glGetUniformLocation(id, name.c_str()), 1, gl::GL_FALSE, mat);
    }

    void ShaderProgram::setUniformMat4(const std::string &name, const gl::GLfloat *mat) const
    {
        gl::glProgramUniformMatrix4fv(id, gl::glGetUniformLocation(id, name.c_str()), 1, gl::GL_FALSE, mat);
    }

    void ShaderProgram::setUniformVec2(const std::string &name, glm::vec2 vec) const
    {
        gl::glProgramUniform2f(id, gl::glGetUniformLocation(id, name.c_str()), vec.x, vec.y);
    }

    void ShaderProgram::setUniformVec3(const std::string &name, glm::vec3 vec) const
    {
        gl::glProgramUniform3f(id, gl::glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z);
    }

    void ShaderProgram::setUniformVec4(const std::string &name, glm::vec4 vec) const
    {
        gl::glProgramUniform4f(id, gl::glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }
    void ShaderProgram::setUniformTex(const std::string &name, gl::GLuint texUnitIndex) const
    {
        gl::glProgramUniform1i(id, gl::glGetUniformLocation(id, name.c_str()), texUnitIndex);
    }
}