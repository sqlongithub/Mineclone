#pragma once
#include <cstdint>
#include "glad/glad.h"

namespace Mineclone {
    class IndexBuffer {
    public:
        IndexBuffer(const void* data, uint32_t count, GLenum type);
        ~IndexBuffer();
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        IndexBuffer(IndexBuffer&& other) noexcept
                : m_type(other.m_type), m_id(other.m_id), m_count(other.m_count)
        {
            other.m_id = 0;
            other.m_count = 0;
        }

        IndexBuffer& operator=(IndexBuffer&& other) noexcept
        {
            if (this != &other) {
                if (m_id != 0)
                    glDeleteBuffers(1, &m_id); // release existing buffer

                m_id = other.m_id;
                m_count = other.m_count;

                other.m_id = 0;
                other.m_count = 0;
            }
            return *this;
        }

        void bind() const;
        void unbind() const;


        [[nodiscard]] GLenum getType() const { return m_type; };
        [[nodiscard]] inline uint32_t getCount() const { return m_count; };
    private:
        GLenum m_type;
        uint32_t m_id;
        uint32_t m_count;
    };

} // Mineclone
