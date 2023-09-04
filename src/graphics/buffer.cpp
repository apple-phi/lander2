#include <glbinding/gl/gl.h>
#include "graphics/buffer.h"

namespace Graphics
{
    VAO::VAO()
    {
        gl::glCreateVertexArrays(1, &id);
        // bind();
    }
    VAO::~VAO()
    {
        // gl::glDeleteVertexArrays(1, &id);
    }
    void VAO::bind() const
    {
        gl::glBindVertexArray(id);
    }
    void VAO::unbind() const
    {
        gl::glBindVertexArray(0);
    }
    VAO VAO::attachVBO(gl::GLuint bindingIndex,
                       gl::GLuint vbo,
                       gl::GLintptr offset,
                       gl::GLsizei stride) const
    {
        gl::glVertexArrayVertexBuffer(id, bindingIndex, vbo, offset, stride);
        return *this;
    }
    VAO VAO::attachEBO(gl::GLuint ebo) const
    {
        gl::glVertexArrayElementBuffer(id, ebo);
        return *this;
    }
    VAO VAO::addVertAttr(gl::GLuint location,
                         gl::GLint bindingIndex,
                         gl::GLint size,
                         gl::GLenum type,
                         gl::GLboolean normalized,
                         gl::GLuint relativeOffset) const
    {
        gl::glVertexArrayAttribFormat(id, location, size, type, normalized, relativeOffset);
        gl::glVertexArrayAttribBinding(id, location, bindingIndex);
        gl::glEnableVertexArrayAttrib(id, location);
        return *this;
    }

    // ------------------------------------------------------------------------

    Buffer::Buffer()
    {
        gl::glCreateBuffers(1, &id);
    }
    Buffer::~Buffer()
    {
        // gl::glDeleteBuffers(1, &id);
    }
    void Buffer::addData(gl::GLsizeiptr size, const void *data, gl::GLenum usage) const
    {
        gl::glNamedBufferData(id, size, data, usage);
    }
    // void Buffer::bind() const
    // {
    //     gl::glBindBuffer(gl::GL_ARRAY_BUFFER, id);
    // }
    // void Buffer::unbind() const
    // {
    //     gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);
    // }
}