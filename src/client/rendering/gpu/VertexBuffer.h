#pragma once
#include <cstdint>
#include <iostream>

#include "VertexAttribute.h"

namespace Mineclone {
    class VertexBuffer {
    public:
        VertexBuffer() = default;
        VertexBuffer(const void* data, uint32_t size);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        VertexBuffer(VertexBuffer&& other) noexcept
          : m_id(other.m_id), m_size(other.m_size) {
            other.m_id = 0;
            other.m_size = 0;
        }
        VertexBuffer& operator=(VertexBuffer&& other) noexcept {
            if (this != &other) {
                if (m_id != 0) glDeleteBuffers(1, &m_id);
                m_id = other.m_id;
                m_size = other.m_size;
                other.m_id = 0;
                other.m_size = 0;
            }
            return *this;
        }

        void setData(const void* data, uint32_t size);

        void bind() const;
        void unbind() const;

        size_t getSize() const { return m_size; };
    private:
        uint32_t m_id = 0;
        size_t m_size = 0;

    };

}
