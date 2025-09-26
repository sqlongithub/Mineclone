#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

namespace Mineclone {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        VertexArray(VertexArray&& other) noexcept : m_id(other.m_id) {
            other.m_id = 0;
        }

        VertexArray& operator=(VertexArray&& other) noexcept {
            if (this != &other) {
                if (m_id != 0) glDeleteVertexArrays(1, &m_id);
                m_id = other.m_id;
                other.m_id = 0;
            }
            return *this;
        }

        void bind() const;
        void unbind() const;

        void addBuffer(const VertexBuffer &vbo, const VertexLayout& layout);

        void addIndexBuffer(const IndexBuffer& ibo);

        [[nodiscard]] unsigned int getId() const { return m_id; }

    private:
        unsigned int m_id = 0;
    };
}
