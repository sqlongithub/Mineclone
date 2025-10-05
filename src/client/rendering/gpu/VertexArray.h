#pragma once
#include "glad/glad.h"
#include "VertexLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <span>

namespace Mineclone {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        void bind() const;
        void unbind() const;

        void addBuffer(const VertexBuffer& vbo, const std::span<const VertexAttribute> layoutAttributes, size_t stride);

        template<size_t N>
        void addBuffer(const VertexBuffer& vbo, const VertexLayout<N>& layout) {
            addBuffer(vbo, layout.getAttributes(), layout.getStride());
        }

        GLuint getId() const { return m_id; }

        void addIndexBuffer(const IndexBuffer& ibo);

    private:
        GLuint m_id = 0;
    };

}
