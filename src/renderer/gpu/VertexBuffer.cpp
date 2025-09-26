#include "glad/glad.h"
#include "VertexBuffer.h"

#include <string>

namespace Mineclone {
    VertexBuffer::VertexBuffer(const void* data, uint32_t size)
        : m_size(size) {
        glGenBuffers(1, &m_id);
#ifdef DEBUG
        std::cout << "creating vertex buffer " << std::to_string(m_id) << ":)\n";
#endif
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()  {
        std::cout << "deleting vertex buffer " << std::to_string(m_id) << ":(\n";
        glDeleteBuffers(1, &m_id);
    }

    void VertexBuffer::setData(const void* data, uint32_t size) {
        if (m_id == 0) {
            glGenBuffers(1, &m_id);
        }
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_size = size;
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}