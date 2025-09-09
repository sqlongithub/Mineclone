#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Mineclone {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();
        void bind() const;
        void unbind() const;
        void addBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

    private:
        unsigned int m_id;
    };
} // Mineclone
