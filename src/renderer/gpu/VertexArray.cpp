#include <iostream>
#include "glad/glad.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace Mineclone {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
        if (m_id == 0) {
            std::cerr << "ERROR: Failed to generate Vertex Array Object!" << std::endl;
        }
    }

    VertexArray::~VertexArray() {
        if (m_id != 0) {
            glDeleteVertexArrays(1, &m_id);
        }
    }

    void VertexArray::bind() const {
        glBindVertexArray(m_id);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::addBuffer(const VertexBuffer &vbo, const VertexLayout& layout) {
        bind();
        vbo.bind();

        const auto& attributes = layout.getAttributes();
        size_t stride = layout.getStride();

        for (unsigned int i = 0; i < attributes.size(); ++i) {
            const auto& attr = attributes[i];
            glEnableVertexAttribArray(i);

            if (attr.type == GL_INT || attr.type == GL_UNSIGNED_INT || attr.type == GL_UNSIGNED_BYTE) {
                glVertexAttribIPointer(
                        i,
                        attr.count,
                        attr.type,
                        static_cast<GLsizei>(stride),
                        reinterpret_cast<const void*>(attr.offset)
                );
            } else {
                glVertexAttribPointer(
                        i,
                        attr.count,
                        attr.type,
                        attr.normalized,
                        static_cast<GLsizei>(stride),
                        reinterpret_cast<const void*>(attr.offset)
                );
            }
        }

        vbo.unbind();
        unbind();
    }

    void VertexArray::addIndexBuffer(const IndexBuffer& ibo) {
        bind();
        ibo.bind();
        unbind();
    }
}