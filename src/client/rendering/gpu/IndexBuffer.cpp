#include "glad/glad.h"
#include "IndexBuffer.h"

#include <iostream>
#include <stdexcept>

namespace Mineclone {
    IndexBuffer::IndexBuffer(const void* data, uint32_t count, GLenum type)
            : m_type(validateIndexType(type)), m_id(0), m_count(count)
    {
        glGenBuffers(1, &m_id);
        if (m_id == 0) {
            throw std::runtime_error("Failed to generate index buffer");
        }

        bind();
        size_t elementSize = getGlEnumSize(m_type);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * elementSize, data, GL_STATIC_DRAW);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error in IndexBuffer constructor: " << std::hex << error << std::endl;
        }
    }

    IndexBuffer::~IndexBuffer() {
        if (m_id != 0) glDeleteBuffers(1, &m_id);
    }

    GLenum IndexBuffer::validateIndexType(GLenum type) {
        switch (type) {
            case GL_UNSIGNED_BYTE:
            case GL_UNSIGNED_SHORT:
            case GL_UNSIGNED_INT:
                return type;
            default:
                std::cerr << "Invalid index type: " << std::hex << type << ". Defaulting to GL_UNSIGNED_INT" << std::endl;
                return GL_UNSIGNED_INT;
        }
    }

    void IndexBuffer::setData(const void* data, uint32_t count, GLenum type) {
        m_type = validateIndexType(type);
        m_count = count;

        if (m_id == 0) {
            glGenBuffers(1, &m_id);
            if (m_id == 0) {
                throw std::runtime_error("Failed to generate index buffer");
            }
        }

        bind();
        size_t elementSize = getGlEnumSize(m_type);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * elementSize, data, GL_STATIC_DRAW);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error in IndexBuffer::setData: " << std::hex << error << std::endl;
        }
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}