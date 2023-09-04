#pragma once
#include <glbinding/gl/gl.h>

namespace Graphics
{
    struct VAO
    {
        gl::GLuint id;
        VAO();
        ~VAO();
        void bind() const;
        void unbind() const;
        VAO attachVBO(gl::GLuint bindingIndex,
                      gl::GLuint buffer,
                      gl::GLintptr offset,
                      gl::GLsizei stride) const;
        VAO attachEBO(gl::GLuint buffer) const;
        VAO addVertAttr(gl::GLuint location,
                        gl::GLint bindingIndex,
                        gl::GLint size,
                        gl::GLenum type,
                        gl::GLboolean normalized,
                        gl::GLuint relativeOffset) const;
    };
    struct Buffer
    {
        gl::GLuint id;
        Buffer();
        ~Buffer();
        void addData(gl::GLsizeiptr size, const void *data, gl::GLenum usage) const;
        // void bind() const;
        // void unbind() const;
    };
}