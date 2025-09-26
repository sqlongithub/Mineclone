#pragma once
#include <cstdint>

#include "VertexAttribute.h"
#include "glad/glad.h"

namespace Mineclone {
    class IndexBuffer {
    public:
        IndexBuffer() = default;
        IndexBuffer(const void* data, uint32_t count, GLenum type);
        ~IndexBuffer();
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        IndexBuffer(IndexBuffer&& other) noexcept
           : m_type(other.m_type), m_id(other.m_id), m_count(other.m_count) {
            other.m_id = 0;
            other.m_count = 0;
            other.m_type = GL_UNSIGNED_INT;
        }

        IndexBuffer& operator=(IndexBuffer&& other) noexcept {
            if (this != &other) {
                if (m_id != 0)
                    glDeleteBuffers(1, &m_id);

                m_id = other.m_id;
                m_count = other.m_count;
                m_type = other.m_type;

                other.m_id = 0;
                other.m_count = 0;
                other.m_type = GL_UNSIGNED_INT;
            }
            return *this;
        }

        static GLenum validateIndexType(GLenum type);

        void setData(const void* data, uint32_t count, GLenum type = GL_UNSIGNED_INT);

        void bind() const;
        void unbind() const;


        [[nodiscard]] GLenum getType() const { return m_type; };
        [[nodiscard]] inline uint32_t getCount() const { return m_count; };

        size_t getSize() const { return getGlEnumSize(m_type) * m_count; };
    private:
        GLenum m_type;
        uint32_t m_id;
        uint32_t m_count;
    };

}
