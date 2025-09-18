#include "UniformBuffer.h"
#include "glad/glad.h"

namespace Mineclone {
    UniformBuffer::UniformBuffer(std::size_t size, uint32_t binding)
            : m_bindingPoint(binding), m_size(size)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_id);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    UniformBuffer::~UniformBuffer() {
        glDeleteBuffers(1, &m_id);
    }

    void UniformBuffer::setData(const void* data, std::size_t size, std::size_t offset) {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBuffer::bind() const {
        glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_id);
    }

    void UniformBuffer::unbind() const {
        glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, 0);
    }
}