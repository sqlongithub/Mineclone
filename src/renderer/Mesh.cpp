#include <iostream>
#include "Mesh.h"
#include "glad/glad.h"

namespace Mineclone {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
            : m_vertexArray(),
              m_vertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex)),
              m_indexBuffer(indices.data(), indices.size()),
              m_vertexCount(vertices.size()),
              m_indexCount(indices.size())
    {
            std::cout << "=== MESH DEBUG INFO ===" << std::endl;
            std::cout << "Vertex count: " << vertices.size() << std::endl;
            std::cout << "Index count: " << indices.size() << std::endl;
            std::cout << "Vertex struct size: " << sizeof(Vertex) << std::endl;
            std::cout << "Expected size: " << (6 * sizeof(float)) << std::endl;

            // Print first few vertices
            for (size_t i = 0; i < std::min(vertices.size(), size_t(4)); i++) {
                std::cout << "Vertex " << i << ": pos("
                          << vertices[i].position.x << ", "
                          << vertices[i].position.y << ", "
                          << vertices[i].position.z << ") color("
                          << vertices[i].color.r << ", "
                          << vertices[i].color.g << ", "
                          << vertices[i].color.b << ")" << std::endl;
            }

            // Print first few indices
            std::cout << "First few indices: ";
            for (size_t i = 0; i < std::min(indices.size(), size_t(12)); i++) {
                std::cout << indices[i] << " ";
            }
            std::cout << std::endl;

            VertexBufferLayout layout;
            layout.push<float>(3); // position
            layout.push<float>(3); // color

            std::cout << "Layout stride: " << layout.getStride() << std::endl;

            m_vertexArray.bind();
            m_vertexArray.addBuffer(m_vertexBuffer, layout);
            m_indexBuffer.bind();
            m_vertexArray.unbind();

            // Verify OpenGL state
            GLint currentVAO;
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
            std::cout << "Current VAO after setup: " << currentVAO << std::endl;

            std::cout << "=== END DEBUG ===" << std::endl;
    }

    Mesh::~Mesh() {

    }

    void Mesh::draw() const {
        m_vertexArray.bind();
        glDrawElements(GL_TRIANGLES, m_indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
        m_vertexArray.unbind();
    }

    uint32_t Mesh::getVertexCount() {
        return m_vertexCount;
    }

    uint32_t Mesh::getIndexCount() {
        return m_indexCount;
    }
}