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

    void VertexArray::addIndexBuffer(const IndexBuffer& ibo) {
        bind();
        ibo.bind();
        unbind();
    }

    void VertexArray::addBuffer(const VertexBuffer& vbo, const std::span<const VertexAttribute> layoutAttributes, size_t stride) {
        bind();
        vbo.bind();

        for (size_t i = 0; i < layoutAttributes.size(); ++i) {
            const auto& attr = layoutAttributes[i];
            glEnableVertexAttribArray(static_cast<GLuint>(i));

            if (attr.isInteger) {
                glVertexAttribIPointer(
                    static_cast<GLuint>(i),
                    attr.count,
                    attr.type,
                    static_cast<GLsizei>(stride),
                    reinterpret_cast<const void*>(attr.offset)
                );
            } else {
                glVertexAttribPointer(
                    static_cast<GLuint>(i),
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
}