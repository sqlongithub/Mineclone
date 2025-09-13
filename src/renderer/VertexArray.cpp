#include <iostream>
#include "glad/glad.h"
#include "VertexArray.h"

namespace Mineclone {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArray::bind() const {
        glBindVertexArray(m_id);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::addBuffer(const Mineclone::VertexBuffer &vbo, const Mineclone::VertexBufferLayout &layout) {
        bind();
        vbo.bind();
        const auto& elements = layout.getElements();

        unsigned int offset = 0;
        for(unsigned int i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(),
                                  reinterpret_cast<const void *>(offset));
            offset += element.count * VertexBufferElement::getSizeOfGLType(element.type);
        }
    }
} // Mineclone