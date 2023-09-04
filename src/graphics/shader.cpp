#include <fstream>
#include <sstream>
#include <iostream>
#include "graphics/shader.h"

namespace Graphics
{
    Shader::Shader(const std::string &filepath, gl::GLenum type) : type(type)
    {
        std::ifstream file(filepath);
        std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        const char *sourcePtr = source.c_str();
        this->id = gl::glCreateShader(type);
        gl::glShaderSource(id, 1, &sourcePtr, NULL);
        gl::glCompileShader(id);
        int success;
        gl::glGetShaderiv(id, gl::GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            gl::glGetShaderInfoLog(id, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED for "
                      << filepath
                      << std::endl;
            throw std::runtime_error(infoLog);
        }
    }
    Shader::~Shader()
    {
        // gl::glDeleteShader(id);
    }
    Shader::operator gl::GLuint() const
    {
        return id;
    }

    // ------------------------------------------------------------------------

    ShaderProgram::ShaderProgram(std::initializer_list<Shader> shaders)
    {
        this->id = gl::glCreateProgram();
        for (auto shader : shaders)
        {
            gl::glAttachShader(id, shader.id);
        }
        gl::glLinkProgram(id);
        int success;
        gl::glGetProgramiv(id, gl::GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            gl::glGetProgramInfoLog(id, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED"
                      << std::endl;
            throw std::runtime_error(infoLog);
        }
    }
    ShaderProgram::~ShaderProgram()
    {
        // gl::glDeleteProgram(id);
    }
    ShaderProgram::operator gl::GLuint() const
    {
        return id;
    }
    void ShaderProgram::use() const
    {
        gl::glUseProgram(id);
    }
    void ShaderProgram::setUniformBool(const std::string &name, bool value) const
    {
        gl::glUniform1i(gl::glGetUniformLocation(id, name.c_str()), (int)value);
    }
    void ShaderProgram::setUniformInt(const std::string &name, int value) const
    {
        gl::glUniform1i(gl::glGetUniformLocation(id, name.c_str()), value);
    }
    void ShaderProgram::setUniformFloat(const std::string &name, float value) const
    {
        gl::glUniform1f(gl::glGetUniformLocation(id, name.c_str()), value);
    }
    void ShaderProgram::setUniformMat3(const std::string &name, const gl::GLfloat *mat) const
    {
        gl::glUniformMatrix3fv(gl::glGetUniformLocation(id, name.c_str()), 1, gl::GL_FALSE, mat);
    }

    void ShaderProgram::setUniformMat4(const std::string &name, const gl::GLfloat *mat) const
    {
        gl::glUniformMatrix4fv(gl::glGetUniformLocation(id, name.c_str()), 1, gl::GL_FALSE, mat);
    }

    void ShaderProgram::setUniformVec2(const std::string &name, glm::vec2 vec) const
    {
        gl::glUniform2f(gl::glGetUniformLocation(id, name.c_str()), vec.x, vec.y);
    }

    void ShaderProgram::setUniformVec3(const std::string &name, glm::vec3 vec) const
    {
        gl::glUniform3f(gl::glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z);
    }

    void ShaderProgram::setUniformVec4(const std::string &name, glm::vec4 vec) const
    {
        gl::glUniform4f(gl::glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }
}