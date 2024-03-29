#pragma once
#include <any>
#include <glbinding/gl/gl.h>

namespace Graphics
{
    struct VAO
    {
        gl::GLuint id;
        VAO();
        explicit VAO(gl::GLuint id);
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
        explicit Buffer(gl::GLuint id);
        ~Buffer();
        void addData(gl::GLsizeiptr size, const void *data, gl::GLenum usage) const;
        gl::GLint getParameter(gl::GLenum pname) const;
        gl::GLsizeiptr getSize() const;
        template <typename T>
        T getSubData(gl::GLintptr offset) const
        {
            T data;
            gl::glGetNamedBufferSubData(id, offset, sizeof(T), &data);
            return std::move(data);
        };
        // void bind() const;
        // void unbind() const;
    };
}